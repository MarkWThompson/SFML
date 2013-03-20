#pragma once

#include <SFML\Network.hpp>
#include "SharedConstants.h"

struct ServerNetworkData
{
	/** The IP address the server is hosting from. */
	sf::IPAddress serverIP;

	/** The clock used to time the interval between the last server and client update. */
	sf::Clock timeoutClock;

	/** The level the server is currently running. */
	std::string serverLevel;

	/** The playerID the client has been assigned. */
	int playerID;

	/** The maximum number of players the server will host for. */
	int maxPlayers;

	/** The receive port of the server. */
	unsigned int serverPort;

	/** Resets server critical variables. */
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
