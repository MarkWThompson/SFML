#pragma once

#include <SFML\Network.hpp>
#include "ServerTransmitter.h"
#include "PacketTypes.h"
#include "SharedConstants.h"
#include "PlayerNetworkData.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Player.h"
#include "Assets.h"
#include "Level.h"
#include "BulletHandler.h"

class GameLogic
{
public:
	/** Default constructor. */
	GameLogic(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData);

	/** Default destructor. */
	~GameLogic();

	/** Main loop. */
	void Update();

	/** Loads the level for the server to host. */
	void LoadLevel(std::string levelFilepath);

	/** Listens for incoming packets and processes any that are valid. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port); // Not used currently, but will be

private:
	/** Container for the player input. */
	struct PlayerInput
	{
		int ID;
		sf::Int16 mouseX, mouseY;
		bool wDown, aDown, sDown, dDown, spaceDown, 
			 escDown, returnDown, lBtnDown, rBtnDown;

		PlayerInput::PlayerInput()
		{
			ID = -1;
			wDown = aDown = sDown = dDown = false;
			spaceDown = escDown = returnDown = false;
			lBtnDown = rBtnDown = false;
			mouseX = mouseY = 0;
		}
	};

	/** Stores the level data. */
	Level level;

	//Class for handling the bullets
	BulletHandler *bulletHandler;

	/** Determines if a player has collided with the level. */
	//bool HitTestObject(sf::Vector2f &player, sf::Rect<float> platformData); // I know this shouldn't be here

	/** Updates a player based on their input. */
	void UpdatePlayer(PlayerInput &playerInput);

	/** Packet output stream. */
	ServerTransmitter* serverTransmitter;

	/** Connected player data. */
	PlayerNetworkData* playerNetworkData;

	/** Stores the authorative update count. */
	sf::Uint32 stateIterator;

	/** Clock used to keep application running at a constant speed. */
	sf::Clock timeStepClock;

	/** Array of the positions of the player sprites, as well as whether they are active or not. */
	std::vector<Player> players;

	/** Stores the player inputs. */
	std::vector<PlayerInput> playerInputs;
};
