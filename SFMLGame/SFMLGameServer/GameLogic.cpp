#include "GameLogic.h"

GameLogic::GameLogic(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData)
{
	this->serverTransmitter = serverTransmitter;
	this->playerNetworkData = playerNetworkData;

	// Temp test code
	spriteXPos = spriteYPos = 0;
	spriteSpeed = 5;

	stateIterator = 0;

	// Start all the clocks, this is fine because when someone connects their clock is reset. I beleive this is probably more efficient than starting stopping them dynamically as I think clocks are done with system time checks, and thus not actively run in active memory. Or maybe they are, what the hell. This is a long comment, well done if you got here, as a reward go to the last line of TextInputBox.h
	for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
	{
		playerNetworkData->playerTimeoutClocks[i].Reset();
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

		if(wDown)
		{
			spriteYPos -= spriteSpeed;
		}

		if(aDown)
		{
			spriteXPos -= spriteSpeed;
		}

		if(sDown)
		{
			spriteYPos += spriteSpeed;
		}

		if(dDown)
		{
			spriteXPos += spriteSpeed;
		}

		PositionPacket positionPacket;
		positionPacket.PackData(sharedConstants.GAME_STATE, spriteXPos, spriteYPos, stateIterator, positionPacket);

		for(int i = 0; i < playerNetworkData->playerIPs.size(); i++)
		{
			if(playerNetworkData->playerIPs[i] != NULL_IP)
			{
				serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), playerNetworkData->playerIPs[i], positionPacket);
			}
		}

		stateIterator++;
	}
}

void GameLogic::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 packetType;
	receivedPacket >> packetType;
	
	if(packetType == PLAYER_INPUT_PACKET)
	{
		receivedPacket >> wDown >> aDown >> sDown >> dDown >> spaceDown >> escDown >> returnDown >> lBtnDown >> rBtnDown >> mouseX >> mouseY;
	}
}
