#pragma once

#include <SFML\Network.hpp>
#include "SharedConstants.h"

struct ServerNetworkData
{
	sf::IPAddress serverIP;
	sf::Clock timeoutClock;
	int playerID;
	int maxPlayers;
	unsigned int serverPort;

	void Reset()
	{
		serverIP = NULL_IP;
		playerID = -1;
		serverPort = -1;
		timeoutClock.Reset();
		maxPlayers = 1;
	}

	ServerNetworkData::ServerNetworkData()
	{
		Reset();
	}
};
