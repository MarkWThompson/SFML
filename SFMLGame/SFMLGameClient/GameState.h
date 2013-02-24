#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientTransmitter.h"
#include "State.h"
#include "PacketTypes.h"
#include "SharedConstants.h"
#include "ServerNetworkData.h"

class GameState : public State
{
public:
	/** Constructor initialiser. */
	GameState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData);

	/** Default destructor. */
	~GameState();

	/** 
	 * Loads GameState content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool Load();
	
	/** Main loop. */
	void Update(sf::Event events, bool eventFired, const sf::Input &input);

	/** Renders all of the applicable state content. */
	void Draw(sf::RenderWindow &renderWindow);

	/** Receives a packet a processes its contents based upon its type. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	/** Checks if the connection to the server has been lost. */
	void CheckTimeout();

	void UnpackPlayerPositionsPacket(sf::Packet &receivedPacket, std::vector<bool> &playersActive, std::vector<sf::Sprite> &playerSprites, sf::Uint32 &stateIterator);

	/** Determines if the client is experiencing packet loss from server. */
	bool connectionProblem;

	/** Used to transmit packets from this state to the server. */
	ClientTransmitter* clientTransmitter;
	ServerNetworkData* serverNetworkData;

	// Game object
	sf::Image playerImage;
	std::vector<sf::Sprite> playerSprites;
	std::vector<bool> playersActive;

	// Connection interrupted
	sf::Image interuptImage;
	sf::Sprite interuptSprite;

	/** The loop counter for the whole fixed update loop, server is authoratative. */
	sf::Uint32 stateIterator;

	/** The clock updates the above stateIterator at the fixed update loop, but can be overwritten by the server, which is authorotative. */
	sf::Clock timeStepClock;
};
