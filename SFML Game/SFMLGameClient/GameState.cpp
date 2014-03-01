#include "GameState.h"
#include "NotificationBox.h"
#include "AudioManager.h"
#include "Stringify.h"

GameState::GameState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData) 
	: State(SharedConstants::GAME_STATE),
	returnButton(1200, 40, 240, 40),
	menuButton(1200, 40, 240, 40)
{
	this->clientTransmitter = clientTransmitter;
	this->serverNetworkData = serverNetworkData;

	stateIterator = 0;
	connectionProblem = false;
	renderCrosshair = true;
	isPaused = false;
	canPause = true;
	//winner = false;

	audioManager.menuMusic.Stop();
	audioManager.gameMusic.Play();

	// Init player scores
	for(size_t i = 0; i < serverNetworkData->maxPlayers; i++)
	{
		playerScores.push_back(0);
	}

	// Init player names
	for(size_t i = 0; i < serverNetworkData->maxPlayers; i++)
	{
		playerNames.push_back("");
	}
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

	renderWindow->ShowMouseCursor(false);
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

	// Load the players
	for(int i = 0; i < serverNetworkData->maxPlayers; i++)
	{
		PlayerSprite player;
		playerSprites.push_back(player);	// Create an empty vector of 8 sprites
		// Set no players to be active at the start
		playersActive.push_back(false);
	}

	// Populate player health array
	for(int i = 0; i < serverNetworkData->maxPlayers; i++)
	{
		playersHealth.push_back(0);
	}

	// Load the projectile Image
	if(!projectileImage.LoadFromFile(PROJECTILE_IMAGE))
	{
		return false;
	}
	/*
	if(!moireFont.LoadFromFile(MOIRE_FONT_FILE))
	{
		return false; 
	}
	else
	{
		winnerText.SetColor(sf::Color::White);
		winnerText.SetSize(30.0f);
	}*/

	// Load the crosshair
	if(crosshairImage.LoadFromFile(CROSSHAIR_IMAGE))
	{
		crosshairSprite.SetImage(crosshairImage); 
	}
	else
	{
		return false;
	}

	// Load pause menu UI
	if(pauseBackingImage.LoadFromFile(PAUSE_BACKING_IMAGE))
	{
		pauseBackingSprite.SetImage(pauseBackingImage); 
	}
	else
	{
		return false;
	}

	if(returnButtonImage.LoadFromFile(RETURN_BUTTON_IMAGE))
	{
		returnButton.SetImage(returnButtonImage);
	}
	else
	{
		return false;
	}

	if(menuButtonImage.LoadFromFile(MENU_BUTTON_IMAGE))
	{
		menuButton.SetImage(menuButtonImage);
	}
	else
	{
		return false;
	}

	// Load the connection problem icon
	if(interuptImage.LoadFromFile(INTERUPT_IMAGE))
	{
		interuptSprite.SetImage(interuptImage); 
	}
	else
	{
		return false;
	}

	// Attempt to load level
	if(!level.Load(serverNetworkData->serverLevel))
	{
		std::cout << "bool GameState::Load() error: failed to load level." << std::endl;
		notificationBox.AddNotification("Fatal error: Failed to load level.", true);
		return false;
	}

	// Will now listen for and receive packets
	StartListening();

	return true;
}

void GameState::Update(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
	// Pause menu toggle
	if(INPUT.IsKeyDown(sf::Key::Escape))
	{
		if(canPause)
		{
			canPause = false;
			isPaused = !isPaused;
		}
	}
	else
	{
		if(canPause == false)
		{
			canPause = true;
		}
	}

	if(isPaused)
	{
		sf::Vector2f viewOrigin(view.GetRect().Left, view.GetRect().Top);

		if(menuButton.Pressed(sf::Mouse::Left, INPUT, viewOrigin))
		{
			SendDisconnectMessage();
			SwitchState(SharedConstants::LOBBY_STATE);
		}

		if(returnButton.Pressed(sf::Mouse::Left, INPUT, viewOrigin))
		{
			isPaused = false;
		}

		menuButton.Update(sf::Mouse::Left, INPUT, viewOrigin);
		returnButton.Update(sf::Mouse::Left, INPUT, viewOrigin);
	}

	if(eventFired == true)
	{
		// sf::Event::MouseEntered seems to have no effect for whatever reason
		// I've check online and can't find anything regarding this
		if(events.Type == sf::Event::MouseMoved)
		{
			renderCrosshair = true;
		}

		if(events.Type == sf::Event::MouseLeft)
		{
			renderCrosshair = false;
		}

		
	}

	if(serverNetworkData->serverIP != NULL_IP)
	{
		// Create input packet
		PlayerInputPacket inputPacket;

		// Grab state of input
		bool wDown = INPUT.IsKeyDown(sf::Key::W);
		bool aDown = INPUT.IsKeyDown(sf::Key::A);
		bool sDown = INPUT.IsKeyDown(sf::Key::S);
		bool dDown = INPUT.IsKeyDown(sf::Key::D);
		bool spaceDown = INPUT.IsKeyDown(sf::Key::Space);
		bool escDown = INPUT.IsKeyDown(sf::Key::Escape);
		bool returnDown = INPUT.IsKeyDown(sf::Key::Return);
		bool lBtnDown = INPUT.IsMouseButtonDown(sf::Mouse::Left);
		bool rBtnDown = INPUT.IsMouseButtonDown(sf::Mouse::Right);
		
		// Get the cursor position relative to view
		const sf::FloatRect viewRect = view.GetRect();
		sf::Int16 mouseX = sf::Int16(viewRect.Left + INPUT.GetMouseX());
		sf::Int16 mouseY = sf::Int16(viewRect.Top + INPUT.GetMouseY());

		// Update crosshair position
		float crosshairX = (float)mouseX - (crosshairImage.GetWidth() / 2);
		float crosshairY = (float)mouseY - (crosshairImage.GetHeight() / 2);
		crosshairSprite.SetPosition(crosshairX, crosshairY);

		// Pack input data
		inputPacket.PackData(sharedConstants.GAME_MODULE, serverNetworkData->playerID, wDown, aDown, sDown, dDown, spaceDown, escDown, returnDown, lBtnDown, rBtnDown, mouseX, mouseY, inputPacket);

		// Send packets
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, inputPacket);
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
		
		// Update player sprites
		for(size_t i = 0; i < playerSprites.size(); i++)
		{
			playerSprites[i].Update(playersHealth[i]);
		}
		/*
		if(winner)
		{
			if(winTimer.GetElapsedTime() >= sharedConstants.GetGracePeriod())
			{
				winner = false;
				winTimer.Reset();
			}
		}*/

		timeStepClock.Reset();
		stateIterator++;
	}

	// Check if the scoreboard should be displayed
	if(INPUT.IsKeyDown(sf::Key::Tab))
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
		UnpackPlayerPositionsPacket(receivedPacket, playersActive, playerSprites, playerDirections, playerViewPoints, stateIterator);
	}
	else if(packetType == PLAYER_DATA_PACKET)
	{
		sf::Uint8 vectorSize;
		receivedPacket >> vectorSize;

		// Unpack player name
		for(int i = 0; i < vectorSize; i++)
		{
			std::string playerName;
			receivedPacket >> playerName;
			playerSprites[i].SetPlayerName(playerName);
			playerNames[i] = playerName;
		}

		// Unpack player scores
		//if(!winner)
		//{
			for(int i = 0; i < vectorSize; i++)
			{
				sf::Int16 playerScore;
				receivedPacket >> playerScore;
				playerScores[i] = playerScore;
			}
			/*
			for(size_t i = 0; i < playerScores.size(); i++)
			{
				if(playerScores[i] == sharedConstants.GetWinningScore())
				{
					winTimer.Reset();
					winner = true;
					winnerText.SetText(playerNames[i] + " won!");
				}
			}*/
		//}

		// Get the player names into a nice vector so you can pass em into the scoreboard
		std::vector<sf::String> playerNames;
		for(size_t i = 0; i < playerSprites.size(); i++)
		{
			playerNames.push_back(playerSprites[i].GetPlayerName());
		}

		// Give the scoreBoard the scores
		scoreBoard.UpdateScores(playerScores, playerNames);

		// Unpack player healths
		for(int i = 0; i < vectorSize; i++)
		{
			sf::Int16 playerHealth = 0;
			receivedPacket >> playerHealth;
			playersHealth[i] = (int)playerHealth;
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

void GameState::UnpackPlayerPositionsPacket(sf::Packet &receivedPacket, std::vector<bool> &playersActive, std::vector<PlayerSprite> &playerSprites, std::vector<int> &playerDirections, std::vector<sf::Vector2f> &playerViewPoints, sf::Uint32 &stateIterator)
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
	sf::Vector2f moveVector;

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
		moveVector.x = playerSprites[i].GetPosition().x - prevPos.x;
		moveVector.y = playerSprites[i].GetPosition().y - prevPos.y;

		playerSprites[i].SetLastMovementVector(moveVector);
	}

	// Grab facing directions
	for(int i = 0; i < numPlayers; i++)
	{
		int facingDirection;
		receivedPacket >> facingDirection;
		playerSprites[i].SetFacingDirection((PlayerSprite::Orientation)facingDirection);
	}

	// Grab view points
	for(int i = 0; i < numPlayers; i++)
	{
		sf::Vector2f viewPoint;
		receivedPacket >> viewPoint;
		playerSprites[i].SetViewPoint(viewPoint);
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
		SwitchState(SharedConstants::SPLASH_STATE);
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
			if(IsOnScreen(*projectileList[i]))
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
			playerSprites[i].Draw(renderWindow);
		}
	}

	level.Draw(Level::FOREGROUND, renderWindow);

	// Scoreboard
	// Work out scoreboard position based on view rect
	if(showScoreBoard)
	{
		scoreBoard.CalculatePosition(view.GetRect());
		scoreBoard.Render(*renderWindow);
	}

	if(isPaused)
	{
		const sf::FloatRect viewPos = view.GetRect();
		pauseBackingSprite.SetPosition(viewPos.Left, viewPos.Bottom - pauseBackingSprite.GetSize().y);
		renderWindow->Draw(pauseBackingSprite);

		menuButton.SetPosition(viewPos.Left + 20.0f, viewPos.Top + 660.0f);
		returnButton.SetPosition(viewPos.Left + 280.0f, viewPos.Top + 660.0f);

		renderWindow->Draw(returnButton);
		renderWindow->Draw(menuButton);
	}

	// Crosshair
	if(renderCrosshair == true)
	{
		renderWindow->Draw(crosshairSprite);
	}

	if(connectionProblem == true)
	{
		// Update icon position
		const sf::FloatRect spritePos = view.GetRect();
		interuptSprite.SetPosition(spritePos.Right - 80.0f, spritePos.Top + 30.0f); // Top right of the screen
		renderWindow->Draw(interuptSprite);
	}

	/*
	if(winner)
	{
		renderWindow->Draw(winnerText);
	}*/
}

bool GameState::IsOnScreen(sf::Sprite sprite)
{
	const sf::FloatRect screenRect = view.GetRect();

	if((sprite.GetPosition().x >= screenRect.Left) && (sprite.GetPosition().x <= screenRect.Right))
	{
		if((sprite.GetPosition().y >= screenRect.Top) && (sprite.GetPosition().y <= screenRect.Bottom))
		{
			return true;
		}
	}
	return false;
}

void GameState::SendDisconnectMessage()
{
	DisconnectionPacket disconnectionPacket;
	disconnectionPacket.PackData(SharedConstants::CONNECT_MODULE, serverNetworkData->playerID, disconnectionPacket);
	clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, disconnectionPacket);
}
