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

	//LevelBounds, this is temporary, should be loaded in from editor
	levelBounds.Left = 0;
	levelBounds.Right = 1400;
	levelBounds.Top = 0;
	levelBounds.Bottom = 900;

	bulletHandler = new BulletHandler(playerNetworkData);
}

GameLogic::~GameLogic()
{
	serverTransmitter = NULL;
	playerNetworkData = NULL;
}

void GameLogic::Update()
{	

	//deal with players that have just spawned
	for(size_t i = 0; i < playerNetworkData->playersToSetup.size(); i++)
	{
		if(playerNetworkData->playersToSetup[i] == true)
		{
			playerNetworkData->playersToSetup[i] = false;
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

		bulletHandler->Update(*serverTransmitter,stateIterator,levelBounds,players,level.GetCollisionBounds());

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
						players[i].HandleCollision(collisionBounds[j]);
					}
				}
			}
		}
		
		//*Pack a player positions packet
		// Create vector of player positions to send in playerPositionsPacket
		std::vector<sf::Vector2f> playerPositions;
		for(size_t i = 0; i < players.size(); i++)
		{
			playerPositions.push_back(players[i].GetPosition());
		}
		PlayerPositionsPacket playerPositionsPacket;
		playerPositionsPacket.PackData(sharedConstants.GAME_STATE, playerNetworkData->MAX_PLAYERS, playerNetworkData->playersActive, playerPositions, stateIterator, playerPositionsPacket);
		///

		//*Pack a player Data Packet
		PlayerDataPacket playerDataPacket;
		//Get the player healths
		std::vector<sf::Int16> playerHealths;
		for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
		{
			playerHealths.push_back(players[i].GetHealth());
		}
		playerDataPacket.PackData(sharedConstants.GAME_STATE, playerNetworkData->MAX_PLAYERS, playerNetworkData->playerNames,playerHealths,playerDataPacket);
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

	int playerID = playerInput.ID;
	
	float xMoved = 0;
	float yMoved = 0;

	// Make sure playerID is valid
	if(playerID >= 0)
	{
		// Set pointer to player instance
		Player& player = players[playerID];

		// Set speeds
		float xSpeed = player.GetXSpeed();
		float ySpeed = player.GetYSpeed();

		if(playerInput.wDown)
		{
			player.MoveDeltaY(-ySpeed);
			yMoved -= players[playerID].GetMoveSpeed().y;
		}
		if(playerInput.sDown)
		{
			player.MoveDeltaY(ySpeed);
			yMoved += players[playerID].GetMoveSpeed().y;
		}
		if(playerInput.aDown)
		{
			player.MoveDeltaX(-xSpeed);
			xMoved -= players[playerID].GetMoveSpeed().x;
		}
		if(playerInput.dDown)
		{
			player.MoveDeltaX(xSpeed);
			xMoved += players[playerID].GetMoveSpeed().x;
		}
		players[playerID].SetLastMovementVector(xMoved,yMoved);
	}

	//Shooting
	if((playerInput.spaceDown) && (players[playerID].CanShoot()))
	{
		sf::Vector2f bulletVelocity;
		bulletVelocity.x = players[playerID].GetFacingDirection() * players[playerID].GetProjectileSpeed().x;
		bulletVelocity.y = players[playerID].GetProjectileSpeed().y;
		bulletHandler->SpawnBullet(players[playerID].GetShootPosition(), bulletVelocity, stateIterator);
	}


	//Score test
	if(playerInput.returnDown)
	{
		players[playerID].SetScore(players[playerID].GetScore() + 1);
	}
}

void GameLogic::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 packetType;
	receivedPacket >> packetType;
	
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
	else if(packetType == SCORE_REQUEST_PACKET)
	{
		//send the vector of scores back
		ScoreResponsePacket scoreResponsePacket;
		std::vector<int> playerScores;
		for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
		{
			if(playerNetworkData->playersActive[i] == true)
			{
				playerScores.push_back(players[i].GetScore());
			}
			else
			{
				playerScores.push_back(0);
			}
		}

		scoreResponsePacket.PackData(sharedConstants.GAME_STATE, playerNetworkData->MAX_PLAYERS, playerScores, scoreResponsePacket);
		serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, scoreResponsePacket);
	}

}

void GameLogic::LoadLevel(std::string levelFilepath)
{
	if(!level.Load(levelFilepath))
	{
		std::cout << "GameLogic::LoadLevel(std::string levelFilepath) error: Failed to load level." << std::endl;
	}
}