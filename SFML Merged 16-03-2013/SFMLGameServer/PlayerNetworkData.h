#pragma once

#include <vector>
#include <iostream>
#include <SFML\Network.hpp>
#include "SharedConstants.h"

struct PlayerNetworkData
{
	int numPlayers;
	const int MAX_PLAYERS;

	std::vector<bool> playersToSetup;
	std::vector<sf::IPAddress> playerIPs;
	std::vector<std::string> playerNames;
	std::vector<bool> playersActive;
	std::vector<sf::Uint32> playerStateIterators;
	std::vector<sf::Clock> playerTimeoutClocks;

	PlayerNetworkData::PlayerNetworkData() : MAX_PLAYERS(8)
	{
		numPlayers = 0;
		playersToSetup.resize(MAX_PLAYERS,false);
		playerNames.resize(MAX_PLAYERS,"Empty");
		playersActive.resize(MAX_PLAYERS, false);
		playerIPs.resize(MAX_PLAYERS, NULL_IP);
		playerStateIterators.resize(MAX_PLAYERS, 0);
		playerTimeoutClocks.resize(MAX_PLAYERS);
	}

	bool PlayerNetworkData::FreeSlot(int ID)
	{
		if((ID >= 0) && (ID < MAX_PLAYERS))
		{
			playerIPs[ID] = NULL_IP;
			playerNames[ID] = "Player";
			playersActive[ID] = false;
			playerStateIterators[ID] = 0;
			playerTimeoutClocks[ID].Reset();
			numPlayers--;
			return true;
		}
		else
		{
			std::cout << "PlayerNetworkData::FreeSlot(int ID) error: invalid player ID." << std::endl;
			return false;
		}
	}
};
