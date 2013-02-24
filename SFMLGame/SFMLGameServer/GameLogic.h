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
	struct PlayerInput
	{
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

		PlayerInput::PlayerInput()
		{
			bool wDown = false;
			bool aDown = false;
			bool sDown = false;
			bool dDown = false;
			bool spaceDown = false;
			bool escDown = false;
			bool returnDown = false;
			bool lBtnDown = false;
			bool rBtnDown = false;
			sf::Int16 mouseX = 0;
			sf::Int16 mouseY = 0;
		}
	};

	void DoPlayerUpdate(std::vector<PlayerInput> &playerInputs);

	/** Packet output stream. */
	ServerTransmitter* serverTransmitter;

	PlayerNetworkData* playerNetworkData;

	sf::Uint32 stateIterator;

	/** Clock used to keep application running at a constant speed. */
	sf::Clock timeStepClock;
	
	sf::Vector2f spawnPosition;
	//Array of the positions of the player sprites, as well as whether they are active or not
	std::vector<sf::Vector2f> playerPositions;

	std::vector<PlayerInput> playerInputs;

	float playerMoveSpeed;
};
