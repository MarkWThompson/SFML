#include "GameState.h"

GameState::GameState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData) 
	: State(SharedConstants::GAME_STATE)
{
	this->clientTransmitter = clientTransmitter;
	this->serverNetworkData = serverNetworkData;
	stateIterator = 0;
	connectionProblem = false;
}

GameState::~GameState()
{
	clientTransmitter = NULL;
	serverNetworkData = NULL;
}

bool GameState::Load()
{
	tempCount = 0;
	workingBulletID = 0;
	scoreBoardRefreshClock.Reset();
	scoreBoardRefreshTime = 3;


	// Load the player
	if(playerImage.LoadFromFile(PLAYER_IMAGE))
	{
		for(int i = 0; i < serverNetworkData->maxPlayers; i++)
		{
			PlayerSprite player;
			playerSprites.push_back(player);	//Create an empty vector of 8 sprites
			playerSprites[i].SetImage(playerImage);
			// Set no players to be active at the start
			playersActive.push_back(false);
		}
	}
	else
	{
		return false;
	}

	//Load the projectile Image
	if(projectileImage.LoadFromFile(PROJECTILE_IMAGE))
	{
	}
	else
	{
		return false;
	}

	// Load the connection problem icon
	if(interuptImage.LoadFromFile(INTERUPT_IMAGE))
	{
		interuptSprite.SetImage(interuptImage); 
		interuptSprite.SetPosition(1200.0f, 30.0f); // Top left of the screen
	}
	else
	{
		return false;
	}

	// Will now listen for and receive packets
	canReceive = true;

	// Attempt to load level
	if(!level.Load(serverNetworkData->serverLevel))
	{
		std::cout << "bool GameState::Load() error: failed to load level." << std::endl;
		return false;
	}

	return true;
}

void GameState::Update(sf::Event events, bool eventFired, const sf::Input &input)
{
	if(serverNetworkData->serverIP != NULL_IP)
	{
		// Create input packet
		PlayerInputPacket inputPacket;

		// Grab state of keys
		bool wDown = input.IsKeyDown(sf::Key::W);
		bool aDown = input.IsKeyDown(sf::Key::A);
		bool sDown = input.IsKeyDown(sf::Key::S);
		bool dDown = input.IsKeyDown(sf::Key::D);
		bool spaceDown = input.IsKeyDown(sf::Key::Space);
		bool escDown = input.IsKeyDown(sf::Key::Escape);
		bool returnDown = input.IsKeyDown(sf::Key::Return);
		bool lBtnDown = input.IsMouseButtonDown(sf::Mouse::Left);
		bool rBtnDown = input.IsMouseButtonDown(sf::Mouse::Right);
		sf::Int16 mouseX = input.GetMouseX();
		sf::Int16 mouseY = input.GetMouseY();

		// Pack input data
		inputPacket.PackData(sharedConstants.GAME_MODULE, serverNetworkData->playerID, wDown, aDown, sDown, dDown, spaceDown, escDown, returnDown, lBtnDown, rBtnDown, mouseX, mouseY, inputPacket);

		// This packet goes to the connectionHandler on the server
		ConnectionValidityPacket validationPacket;
		validationPacket.PackData(SharedConstants::CONNECT_MODULE, serverNetworkData->playerID, stateIterator, true, validationPacket);

		// Send packets
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, inputPacket);
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, validationPacket);

		// Handle player sprite orientation
		for(size_t i = 0; i < playerSprites.size(); i++)
		{
			playerSprites[i].HandleOrientation();
		}
	}

	// The fixed timestep on the client
	if(timeStepClock.GetElapsedTime() > sharedConstants.GetTimeStep())
	{
		//update bullets
		for(size_t i = 0; i < projectileList.size(); i++)
		{
			projectileList[i]->update();
			if(projectileList[i]->pleaseErase == true)
			{
				projectileList.erase(projectileList.begin() + i);
				continue;
			}
		}

		timeStepClock.Reset();
		stateIterator++;
	}

	//check if you should display scoreboard
	if(input.IsKeyDown(sf::Key::Tab))
	{
		showScoreBoard = true;


		if((shouldGetScores == true) || (scoreBoardRefreshClock.GetElapsedTime() > scoreBoardRefreshTime))
		{
			scoreBoardRefreshClock.Reset();
			shouldGetScores = false;
			ScoreRequestPacket scoreRequestPacket;
			scoreRequestPacket.PackData(sharedConstants.GAME_MODULE,scoreRequestPacket);
			clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, scoreRequestPacket);
		}
	}
	else
	{
		showScoreBoard = false;
		shouldGetScores = true;
	}

	CheckTimeout();
}

void GameState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	// Has received data, reset timeout clock
	serverNetworkData->timeoutClock.Reset();

	// Grab packet type
	sf::Uint8 packetType;
	receivedPacket >> packetType;

	if(packetType == PLAYER_POSITIONS_PACKET)
	{
		UnpackPlayerPositionsPacket(receivedPacket, playersActive, playerSprites, stateIterator);
	}
	else if(packetType == PLAYER_DATA_PACKET)
	{
		sf::Uint8 vectorSize;
		receivedPacket >> vectorSize;
		//unpack player name
		for(int i = 0; i < vectorSize; i++)
		{
			std::string playerName;
			receivedPacket >> playerName;
			playerSprites[i].SetPlayerName(playerName);
		}
		//unpack player healths
		for(int i = 0; i < vectorSize; i++)
		{
			sf::Int16 playerHealth = 0;
			receivedPacket >> playerHealth;
			playerSprites[i].UpdateHealthBarHealth(playerHealth);
		}
	}
	else if(packetType == PROJECTILE_PACKET)
	{
		UnpackProjectilePacket(receivedPacket);
	}
	else if(packetType == PROJECTILE_DEATH_PACKET)
	{
		UnpackProjectileDeathPacket(receivedPacket);
	}
	else if(packetType == SCORE_RESPONSE_PACKET)
	{
		sf::Uint8 vectorSize;
		std::vector<int> scores;
		receivedPacket >> vectorSize;
		
		//unpack player scored into scores vector
		for(size_t i = 0; i < vectorSize; i++)
		{
			int extractedScore;
			receivedPacket >> extractedScore;
			scores.push_back(extractedScore);
		}

		//Get the player names into a nice vector so you can pass em into the scoreboard
		std::vector<sf::String> playerNames;
		for(size_t i = 0; i < playerSprites.size(); i++)
		{
			playerNames.push_back(playerSprites[i].GetPlayerName());
		}

		scoreBoard.UpdateScores(scores,playerNames);
	}
	else
	{
		std::cout << "Packet type: UNDEFINED_PACKET" << std::endl;
	}
}

void GameState::UnpackPlayerPositionsPacket(sf::Packet &receivedPacket, std::vector<bool> &playersActive, std::vector<PlayerSprite> &playerSprites, sf::Uint32 &stateIterator)
{
	// Get the size of the playerSprites vector size and store as the number of players
	sf::Uint8 numPlayers;
	receivedPacket >> numPlayers;

	// Grab what players are active
	for(int i = 0; i < numPlayers; i++)
	{
		bool isActive = false;
		receivedPacket >> isActive;
		playersActive[i] = isActive;
	}

	// Grab coords
	for(int i = 0; i < numPlayers; i++)
	{
		// Get previous position
		sf::Vector2f prevPos = playerSprites[i].GetPosition();

		// Get new position
		sf::Vector2f newPos;
		receivedPacket >> newPos;

		// Calculate displacement vector
		sf::Vector2f lastMovementVector = newPos - prevPos;

		// Set current position and displacement
		playerSprites[i].SetPosition(newPos);
		playerSprites[i].SetLastMovementVector(lastMovementVector.x, lastMovementVector.y);
	}

	// Set state iteration variable
	sf::Uint32 stateIteration;
	receivedPacket >> stateIteration;
	stateIterator = stateIteration;
}

void GameState::UnpackProjectilePacket(sf::Packet &receivedPacket)
{
	sf::Uint32 bulletID;
	sf::Vector2f bulletPosition;
	sf::Vector2f bulletVelocity;

	receivedPacket >> bulletID >> bulletPosition >> bulletVelocity;
	
	//std::cout << bulletID << std::endl;

	//If the bulletID is less than or equal to the working one, it might be on the list, so scan through and check. This could be sped up by keeping track of destroyed bullets, and doing workingBulletID - destroyedBullets, however i have tested, and even with the entire upper if commented out you get at most 5-10 more bullets before noticeable lag
	if(bulletID < workingBulletID)
	{
		//std::cout << "Searching bullet list" << std::endl;
		tempCount++;
		for(size_t i = 0; i < projectileList.size(); i++)
		{
			//If this bullet is already on the list
			if(bulletID == projectileList[i]->bulletID)
			{	
				projectileList[i]->SetPosition(bulletPosition);
				projectileList[i]->SetVelocity(bulletVelocity.x, bulletVelocity.y);
				return;
			}
		}
	}
	else if(bulletID >= workingBulletID)
	{
		//std::cout << "Creating new bullet" << std::endl;
		//if we get here the bullet is not on the list
		BulletSprite* bulletToPush = new BulletSprite(bulletID);
		bulletToPush->SetPosition(bulletPosition);
		bulletToPush->SetVelocity(bulletVelocity.x, bulletVelocity.y);
		bulletToPush->SetImage(projectileImage);
		projectileList.push_back(bulletToPush);
		workingBulletID++;
	}
	else
	{
		std::cout << "Problem in UnpackPlayerProjectiles in GameState";
	}
}

void GameState::UnpackProjectileDeathPacket(sf::Packet &receivedPacket)
{
	sf::Uint32 bulletID;
	sf::Vector2f deathPosition;

	receivedPacket >> bulletID >> deathPosition;

	//scan the vector, god this must be so inneficient
	for(size_t i = 0; i < projectileList.size(); i++)
	{
		if(projectileList[i]->bulletID == bulletID)
		{
			projectileList.erase(projectileList.begin() + i);
			return;
		}
	}
}


void GameState::CheckTimeout()
{
	// If the time since the last packet received is greater than the timeout indication time, flag a connection problem
	(serverNetworkData->timeoutClock.GetElapsedTime() > sharedConstants.GetTimeoutIndicationTime()) ? connectionProblem = true : connectionProblem = false;

	// Timeout
	if(serverNetworkData->timeoutClock.GetElapsedTime() > sharedConstants.GetTimeout())
	{
		serverNetworkData->Reset();

		std::cout << "Lost connetion to the server, now disconnecting..." << std::endl;

		// Go back to start state
		targetID = sharedConstants.START_STATE;
	}
}

void GameState::Draw(sf::RenderWindow &renderWindow)
{

	//would be nice to seperate this into drawBackground, drawPlatforms and drawForeground functions so we can have stuff render between level layers
	level.Draw(renderWindow);

	//Render the bullets
	for(size_t i = 0; i < projectileList.size(); i++)
	{
		if(projectileList[i] != NULL)
		{
			if(IsOnScreen(renderWindow,*projectileList[i]))
			{
				renderWindow.Draw(*projectileList[i]);
			}
		}
	}

	// Render all the players
	for(int i = 0; i < serverNetworkData->maxPlayers; i++)
	{
		if(playersActive[i] == true)
		{
			playerSprites[i].DrawExtras(renderWindow);
			renderWindow.Draw(playerSprites[i]);
		}
	}

	//Scoreboard
	if(showScoreBoard)
	{
		scoreBoard.Render(renderWindow);
	}

	if(connectionProblem == true)
	{
		renderWindow.Draw(interuptSprite);
	}
}

bool GameState::IsOnScreen(sf::RenderWindow &renderWindow, sf::Sprite &sprite)
{
	if((sprite.GetPosition().x > 0) && (sprite.GetPosition().x < renderWindow.GetWidth()))
	{
		if((sprite.GetPosition().y > 0) && (sprite.GetPosition().y < renderWindow.GetHeight()))
		{
			return true;
		}
	}
	return false;
}
