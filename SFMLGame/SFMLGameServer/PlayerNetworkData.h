#pragma once

#include <vector>
#include <SFML\Network.hpp>
#include "SharedConstants.h"

struct PlayerNetworkData
{
	int numPlayers;
	const int MAX_PLAYERS;

	std::vector<sf::IPAddress> playerIPs;
	std::vector<sf::Uint32> playerStateIterators;
	std::vector<sf::Clock> playerTimeoutClocks;

	PlayerNetworkData::PlayerNetworkData() : MAX_PLAYERS(8)
	{
		numPlayers = 0;
		playerIPs.resize(MAX_PLAYERS, NULL_IP);
		playerStateIterators.resize(MAX_PLAYERS, 0);
		playerTimeoutClocks.resize(MAX_PLAYERS);
	}
};
