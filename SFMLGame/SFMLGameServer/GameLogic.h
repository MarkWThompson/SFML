#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"
#include "PacketTypes.h"
#include "SharedConstants.h"
#include "PlayerNetworkData.h"

class GameLogic
{
public:
	/** Constructor. */
	GameLogic(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData);

	/** Default destructor. */
	~GameLogic();

	/** Main loop. */
	void Update();

	/** Listens for incoming packets and processes any that are valid. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port); // Not used currently, but will be

private:
	/** Packet output stream. */
	ServerTransmitter* serverTransmitter;

	PlayerNetworkData* playerNetworkData;

	sf::Uint32 stateIterator;

	/** Clock used to keep application running at a constant speed. */
	sf::Clock timeStepClock;

	// Temp test code
	int spriteXPos;
	int spriteYPos;
	int spriteSpeed;

	// Temporary storage for packet data
	bool wDown;
	bool aDown;
	bool sDown;
	bool dDown;
	bool spaceDown;
	bool escDown;
	bool returnDown;
	bool lBtnDown;
	bool rBtnDown;
	sf::Int16 mouseX;
	sf::Int16 mouseY;
};
