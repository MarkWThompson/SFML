#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"
#include "..\PacketTypes.h"

class ConnectionHandler
{
public:
	/** Constructor. */
	ConnectionHandler(ServerTransmitter &serverTransmitter);

	/** Default destructor. */
	~ConnectionHandler();

	/** Listens for packets and processes valid packet content. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	/** Checks if connecting player can connect. */
	void ValidateConnection(sf::IPAddress connectionAddress, unsigned int port);
	
	/** Packet output stream. */
	ServerTransmitter* serverTransmitter;

	/** A vector of connected player IP addresses. */
	std::vector<sf::IPAddress> playerIPs;

	/** The maximum number of players that can join the server. */
	static const int MAX_NUM_PLAYERS = 8;

	/** Current number of players connected to the server. */
	int numPlayers;
};
