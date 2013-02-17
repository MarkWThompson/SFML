#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"
#include "..\PacketTypes.h"
#include "..\SharedConstants.h"

class GameLogic
{
public:
	/** Constructor. */
	GameLogic(ServerTransmitter &serverTransmitter);

	/** Default destructor. */
	~GameLogic();

	/** Main loop. */
	void Update();

	/** Listens for incoming packets and processes any that are valid. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port); // Not used currently, but will be

private:
	/** Packet output stream. */
	ServerTransmitter* serverTransmitter;

	/** Clock used to keep application running at a constant speed. */
	sf::Clock timeStepClock;

	// Temp test code
	sf::IPAddress clientIP;
	int spriteXPos;
	int spriteYPos;
	bool travellingRight;
	int spriteSpeed;
};
