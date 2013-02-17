#include "GameLogic.h"

GameLogic::GameLogic(ServerTransmitter &serverTransmitter)
{
	this->serverTransmitter = &serverTransmitter;
	
	// Temp test code
	clientIP = sf::IPAddress::GetLocalAddress();
	spriteXPos = spriteYPos = 0;
	spriteSpeed = 5;
	travellingRight = true;
}

GameLogic::~GameLogic()
{
	delete serverTransmitter;
	serverTransmitter = NULL;
}

void GameLogic::Update()
{
	// The fixed timestep
	if(timeStepClock.GetElapsedTime() > sharedConstants.GetTimeStep())
	{
		timeStepClock.Reset();

		// I know this should be a #define reference or something, but because of the structure it is unfeasable
		if(travellingRight)
		{
			if(spriteXPos < 1030)
			{
				spriteXPos += spriteSpeed;
			}
			else
			{
				travellingRight = false;
			}
		}
		else
		{
			if(spriteXPos > 0)
			{
				spriteXPos -= spriteSpeed;
			}
			else
			{
				travellingRight = true;
			}
		}

		PositionPacket positionPacket;
		positionPacket.PackData(sharedConstants.GAME_STATE, spriteXPos, spriteYPos, positionPacket);
	
		serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), clientIP, positionPacket);
	}
}

void GameLogic::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{

}
