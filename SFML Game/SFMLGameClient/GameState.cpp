#include "GameState.h"

GameState::GameState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData) 
	: State(SharedConstants::GAME_STATE)
{
	this->clientTransmitter = clientTransmitter;
	this->serverNetworkData = serverNetworkData;
	stateIterator = 0;
	connectionProblem = false;
}

void GameState::SetView(sf::RenderWindow* renderWindow)
{
	// Get view vars
	const sf::Vector2f playerCentre(30.0f, 34.0f); // I know this should be from PlayerSprite but it's not there
	const sf::Vector2f halfSize((float)renderWindow->GetWidth()/2, (float)renderWindow->GetHeight()/2); // Not sure how to get the dimensions of the screen from Main.cpp

	// Initialise view
	view.SetHalfSize(halfSize);
	view.SetCenter(playerCentre);

	// Make the renderWindow use the view
	renderWindow->SetView(view);

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

	// Load the projectile Image
	if(!projectileImage.LoadFromFile(PROJECTILE_IMAGE))
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

		// Send packets
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, inputPacket);

		// Handle player sprite orientation
		for(size_t i = 0; i < playerSprites.size(); i++)
		{
			playerSprites[i].HandleOrientation();
		}
	}

	// The fixed timestep on the client
	if(timeStepClock.GetElapsedTime() > sharedConstants.GetTimeStep())
	{
		// Update bullets
		for(size_t i = 0; i < projectileList.size(); i++)
		{
			projectileList[i]->Update();
			if(projectileList[i]->IsActive() == false)
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
	}
	else
	{
		showScoreBoard = false;
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
		//unpack player scores
		std::vector<sf::Int16> playerScores;
		for(int i = 0; i < vectorSize; i++)
		{
			sf::Int16 playerScore;
			receivedPacket >> playerScore;
			playerScores.push_back(playerScore);
		}

		//Get the player names into a nice vector so you can pass em into the scoreboard
		std::vector<sf::String> playerNames;
		for(size_t i = 0; i < playerSprites.size(); i++)
		{
			playerNames.push_back(playerSprites[i].GetPlayerName());
		}
		//give the scoreBoard the scores
		scoreBoard.UpdateScores(playerScores,playerNames);

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

	// Set vars
	sf::Vector2f prevPos;
	sf::Vector2f newPos;

	// Grab coords
	for(int i = 0; i < numPlayers; i++)
	{
		// Set previous position
		prevPos = playerSprites[i].GetPosition();

		// Set new position
		receivedPacket >> newPos;

		// Set current position and displacement
		playerSprites[i].SetPosition(newPos);
		
		// Calculate displacement vector
		float movementVectorX = playerSprites[i].GetPosition().x - prevPos.x;
		float movementVectorY = playerSprites[i].GetPosition().y - prevPos.y;

		playerSprites[i].SetLastMovementVector(movementVectorX, movementVectorY);
	}

	// Update camera
	int ID = serverNetworkData->playerID;
	view.Move(playerSprites[ID].GetLastMovementVector().x, playerSprites[ID].GetLastMovementVector().y);

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
			if(bulletID == projectileList[i]->GetID())
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
		if(projectileList[i]->GetID() == bulletID)
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

void GameState::Draw(sf::RenderWindow* renderWindow)
{	
	level.Draw(Level::BACKGROUND, renderWindow);

	// Render the bullets
	for(size_t i = 0; i < projectileList.size(); i++)
	{
		if(projectileList[i] != NULL)
		{
			if(IsOnScreen(renderWindow, *projectileList[i]))
			{
				renderWindow->Draw(*projectileList[i]);
			}
		}
	}

	level.Draw(Level::PLATFORMS, renderWindow);

	// Render all the players
	for(int i = 0; i < serverNetworkData->maxPlayers; i++)
	{
		if(playersActive[i] == true)
		{
			playerSprites[i].DrawExtras(*renderWindow);
			renderWindow->Draw(playerSprites[i]);
		}
	}

	level.Draw(Level::FOREGROUND, renderWindow);

	//Scoreboard
	//Work out scoreboard position based on view rect
	if(showScoreBoard)
	{
		scoreBoard.CalculatePosition(view.GetRect());
		scoreBoard.Render(*renderWindow);
	}

	if(connectionProblem == true)
	{
		renderWindow->Draw(interuptSprite);
	}
}

bool GameState::IsOnScreen(sf::RenderWindow* renderWindow, sf::Sprite &sprite)
{
	const sf::FloatRect screenRect = renderWindow->GetView().GetRect();

	if((sprite.GetPosition().x > screenRect.Left) && (sprite.GetPosition().x < screenRect.Right))
	{
		if((sprite.GetPosition().y > screenRect.Top) && (sprite.GetPosition().y < screenRect.Bottom))
		{
			return true;
		}
	}
	return false;
}
