#pragma once

#include <SFML\Network.hpp>
#include "SharedConstants.h"

struct ServerNetworkData
{
	sf::IPAddress serverIP;
	sf::Clock timeoutClock;
	std::string serverLevel;
	int playerID;
	int maxPlayers;
	unsigned int serverPort;

	void Reset()
	{
		serverIP = NULL_IP;
		playerID = serverPort = maxPlayers = -1;
		timeoutClock.Reset();
		serverLevel = "";
	}

	ServerNetworkData::ServerNetworkData()
	{
		Reset();
	}
};
