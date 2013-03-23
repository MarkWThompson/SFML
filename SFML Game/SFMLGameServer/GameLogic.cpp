#include "GameLogic.h"

GameLogic::GameLogic(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData)
{
	this->serverTransmitter = serverTransmitter;
	this->playerNetworkData = playerNetworkData;

	stateIterator = 0;

	// Create generic objects to fill vectors with
	Player player;
	PlayerInput playerInput;

	// Initialise player vectors and network data
	for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
	{
		// Start all the clocks, this is fine because when someone connects their clock is reset. I beleive this is probably more efficient than starting stopping them dynamically as I think clocks are done with system time checks, and thus not actively run in active memory. Or maybe they are, what the hell.
		playerNetworkData->playerTimeoutClocks[i].Reset();
		
		// Fill vectors
		players.push_back(player);
		playerInputs.push_back(playerInput);
	}

	bulletHandler = new BulletHandler(playerNetworkData);
}

GameLogic::~GameLogic()
{
	serverTransmitter = NULL;
	playerNetworkData = NULL;
}

void GameLogic::Update()
{	
	// Deal with players that have just spawned
	for(size_t i = 0; i < playerNetworkData->playersToSetup.size(); i++)
	{
		if(playerNetworkData->playersToSetup[i] == true)
		{
			playerNetworkData->playersToSetup[i] = false;
			playerNetworkData->playersActive[i] = true;
			players[i].SetSpawnPosition(level.GetSpawnPoints()[rand() % level.GetSpawnPoints().size()]);
			players[i].Reset();
		}
	}

	bulletHandler->SendPendingBullets(*serverTransmitter);

	// The fixed timestep
	if(timeStepClock.GetElapsedTime() > sharedConstants.GetTimeStep())
	{
		timeStepClock.Reset();

		// Grab the playerinputs, might not be neccesary, but this does seperate the useable data from the incoming data stream
		for(size_t i = 0; i < playerInputs.size(); i++)
		{
			if(playerNetworkData->playersActive[i])
			{
				UpdatePlayer(playerInputs[i]);
			}
		}

		bulletHandler->Update(*serverTransmitter, stateIterator, level.GetBounds(), players, level.GetCollisionBounds());

		// Execute player main update loop
		for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
		{
			if(playerNetworkData->playersActive[i] == true)
			{
				players[i].Update();
			}
		}

		// Collision detection
		std::vector<sf::Rect<float>> collisionBounds = level.GetCollisionBounds();
		for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
		{
			if(playerNetworkData->playersActive[i] == true)
			{
				for(size_t j = 0; j < collisionBounds.size(); j++)
				{
					if(players[i].GetBounds().Intersects(collisionBounds[j]))
					{
						if(level.GetCollisionBoundsLethality()[j] == true)
						{
							players[i].Die();
							playerNetworkData->playersActive[i] = false;
						}
						else
						{
							players[i].HandleCollision(collisionBounds[j]);
						}
					}
				}
			}
		}
		
		// Check for player death
		for(size_t i = 0; i < players.size(); i++)
		{
			// Check for dead players
			if((players[i].GetHealth() <= 0) && (players[i].GetIsAlive() == true))
			{
				players[i].Die();
				playerNetworkData->playersActive[i] = false;
				players[i].IncreaseScore(-1);
			}
			// Check if its time to respawn
			if((players[i].deathTimer.GetElapsedTime() > players[i].deathTime) && (players[i].GetIsAlive() == false))
			{
				playerNetworkData->playersToSetup[i] = true;
			}
		}

		//*Pack a player positions packet
		// Create vector of player positions to send in playerPositionsPacket
		std::vector<sf::Vector2f> playerPositions;
		for(size_t i = 0; i < players.size(); i++)
		{
			playerPositions.push_back(players[i].GetPosition());
		}
		std::vector<int> playerDirections;
		for(size_t i = 0; i < players.size(); i++)
		{
			playerDirections.push_back((int)players[i].GetFacingDirection());
		}
		PlayerPositionsPacket playerPositionsPacket;
		playerPositionsPacket.PackData(sharedConstants.GAME_STATE, playerNetworkData->MAX_PLAYERS, playerNetworkData->playersActive, playerPositions, playerDirections, stateIterator, playerPositionsPacket);
		///

		//*Pack a player Data Packet
		PlayerDataPacket playerDataPacket;
		//Get the player healths
		std::vector<sf::Int16> playerHealths;
		for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
		{
			playerHealths.push_back(players[i].GetHealth());
		}
		//Get the player scores
		std::vector<sf::Int16> playerScores;
		for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
		{
			playerScores.push_back(players[i].GetScore());
		}
		playerDataPacket.PackData(sharedConstants.GAME_STATE, playerNetworkData->MAX_PLAYERS, playerNetworkData->playerNames,playerScores,playerHealths,playerDataPacket);
		///

		for(size_t i = 0; i < playerNetworkData->playerIPs.size(); i++)
		{
			if(playerNetworkData->playerIPs[i] != NULL_IP)
			{
				serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[i], playerPositionsPacket);
				serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[i], playerDataPacket);
			}
		}

		stateIterator++;
	}
}

void GameLogic::UpdatePlayer(PlayerInput &playerInput)
{
	int playerID =  playerInput.ID;

	// Make sure playerID is valid
	if(playerID >= 0)
	{
		Player& player = players[playerID];

		// Player jump
		if(playerInput.wDown)
		{
			player.Jump();
		}

		// Player move horizontally
		if(playerInput.aDown)
		{
			player.MoveLeft();
		}
		else if(playerInput.dDown)
		{
			player.MoveRight();
		}

		// Last movement vector
		float yMoved = -player.GetYVelocity();
		float xMoved = player.GetXVelocity();
		player.SetLastMovementVector(xMoved, yMoved);
	
		// Direction 
		if(playerInput.mouseX > player.GetPosition().x)
		{
			player.SetFacingDirection(Object::RIGHT);
		}
		else if(playerInput.mouseX < player.GetPosition().x)
		{
			player.SetFacingDirection(Object::LEFT);
		}

		// Shooting
		if((playerInput.lBtnDown) && (player.CanShoot()))
		{
			sf::Vector2f bulletVelocity;

			sf::Vector2f playerCentre;
			playerCentre.x = player.GetPosition().x + (player.GetWidth() / 2);
			playerCentre.y = player.GetPosition().y + (player.GetHeight() / 2);

			float x2 = playerInput.mouseX;
			float x1 = playerCentre.x;
			float y2 = playerInput.mouseY;
			float y1 = playerCentre.y;
			float hyp = player.GetProjectileSpeed();

			float theta = atan2((y2-y1), (x2-x1));
			float y = sin(theta) * hyp;
			float x = cos(theta) * hyp;

			bulletVelocity.x = x;
			bulletVelocity.y = y;

			sf::Vector2f shootPosition;
			shootPosition.x = playerCentre.x + (x * 3.25f); // 3.25f represents barel length
			shootPosition.y = playerCentre.y + (y * 3.25f);

			bulletHandler->SpawnBullet(playerID, shootPosition, bulletVelocity, stateIterator);
		}

		// Score test
		if(playerInput.returnDown)
		{
			player.IncreaseScore(1);
		}
	}
}

void GameLogic::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 packetType;
	receivedPacket >> packetType;
	
	// Reset active player timeout clock
	for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
	{
		if(playerNetworkData->playerIPs[i] == connectionAddress)
		{			
			playerNetworkData->playerIPs[i] = connectionAddress;
			playerNetworkData->playerTimeoutClocks[i].Reset();
		}
	}

	if(packetType == PLAYER_INPUT_PACKET)
	{
		// Get player ID
		int playerID;
		receivedPacket >> playerID;

		// Set pointer to playerInput instance
		PlayerInput& playerInput = playerInputs[playerID];

		// Set ID
		playerInput.ID = playerID;
		
		// Set playerInput
		receivedPacket	>> playerInput.wDown 
						>> playerInput.aDown 
						>> playerInput.sDown 
						>> playerInput.dDown
						>> playerInput.spaceDown
						>> playerInput.escDown
						>> playerInput.returnDown
						>> playerInput.lBtnDown
						>> playerInput.rBtnDown
						>> playerInput.mouseX
						>> playerInput.mouseY;
	}

}

void GameLogic::LoadLevel(std::string levelFilepath)
{
	if(!level.Load(levelFilepath))
	{
		std::cout << "GameLogic::LoadLevel(std::string levelFilepath) error: Failed to load level." << std::endl;
	}
}
