#include "GameLogic.h"

GameLogic::GameLogic(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData)
{
	this->serverTransmitter = serverTransmitter;
	this->playerNetworkData = playerNetworkData;

	stateIterator = 0;

	playerMoveSpeed = 5;
	//The position players go to when they are inactive
	spawnPosition = sf::Vector2f(0.0f,0.0f);

	for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
	{
		// Start all the clocks, this is fine because when someone connects their clock is reset. I beleive this is probably more efficient than starting stopping them dynamically as I think clocks are done with system time checks, and thus not actively run in active memory. Or maybe they are, what the hell.
		playerNetworkData->playerTimeoutClocks[i].Reset();
		//Set up the playerInputs
		PlayerInput playerInput;
		playerInputs.push_back(playerInput);
		//Set up the actual player positions
		playerPositions.push_back(spawnPosition);
	}
}

GameLogic::~GameLogic()
{
	serverTransmitter = NULL;
	playerNetworkData = NULL;
}

void GameLogic::Update()
{	
	// The fixed timestep
	if(timeStepClock.GetElapsedTime() > sharedConstants.GetTimeStep())
	{
		timeStepClock.Reset();

		//Grab the playerinputs, might not be neccesary, but this does seperate the useable data from the incoming data stream
		std::vector<PlayerInput> playerInputsSnapShot;
		for(int i = 0; i < playerInputs.size(); i++)
		{
			playerInputsSnapShot.push_back(playerInputs[i]);
		}

		DoPlayerUpdate(playerInputsSnapShot);


		PlayerPositionsPacket playerPositionsPacket;
		playerPositionsPacket.PackData(sharedConstants.GAME_STATE,playerNetworkData->MAX_PLAYERS,playerNetworkData->playersActive,playerPositions,stateIterator,playerPositionsPacket);


		for(int i = 0; i < playerNetworkData->playerIPs.size(); i++)
		{
			if(playerNetworkData->playerIPs[i] != NULL_IP)
			{
				serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[i], playerPositionsPacket);
			}
		}

		stateIterator++;
	}
}

void GameLogic::DoPlayerUpdate(std::vector<PlayerInput> &playerInputs)
{
	for(int i = 0; i < playerPositions.size(); i++)
	{
		if(playerNetworkData->playersActive[i] == true)
		{
			if(playerInputs[i].wDown)
			{
				playerPositions[i].y -= playerMoveSpeed;
			}
			if(playerInputs[i].sDown)
			{
				playerPositions[i].y += playerMoveSpeed;
			}
			if(playerInputs[i].aDown)
			{
				playerPositions[i].x -= playerMoveSpeed;
			}
			if(playerInputs[i].dDown)
			{
				playerPositions[i].x += playerMoveSpeed;
			}
		}
		else
		{
			playerPositions[i].x = spawnPosition.x;
			playerPositions[i].y = spawnPosition.y;
		}
	}
}


void GameLogic::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 packetType;
	receivedPacket >> packetType;
	
	if(packetType == PLAYER_INPUT_PACKET)
	{
		//receivedPacket >> wDown >> aDown >> sDown >> dDown >> spaceDown >> escDown >> returnDown >> lBtnDown >> rBtnDown >> mouseX >> mouseY;
		int playerID;
		receivedPacket >> playerID;
		
		receivedPacket >> playerInputs[playerID].wDown 
		>> playerInputs[playerID].aDown 
		>> playerInputs[playerID].sDown 
		>> playerInputs[playerID].dDown
		>> playerInputs[playerID].spaceDown
		>> playerInputs[playerID].escDown
		>> playerInputs[playerID].returnDown
		>> playerInputs[playerID].lBtnDown
		>> playerInputs[playerID].rBtnDown
		>> playerInputs[playerID].mouseX
		>> playerInputs[playerID].mouseY;
		

	}
}
