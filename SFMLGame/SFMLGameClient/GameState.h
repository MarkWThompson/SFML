#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientTransmitter.h"
#include "State.h"
#include "..\PacketTypes.h"

class GameState : public State
{
public:
	/** Constructor initialiser. */
	GameState(ClientTransmitter* clientTransmitter);

	/** Default destructor. */
	~GameState();

	/** 
	 * Loads GameState content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool Load();
	
	/** Main loop. */
	void Update(sf::Event events, const sf::Input &input);

	/** Renders all of the applicable state content. */
	void Draw(sf::RenderWindow &renderWindow);

	/** Receives a packet a processes its contents based upon its type. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	/** Used to transmit packets from this state to the server. */
	ClientTransmitter* clientTransmitter;

	// Game object
	sf::Image androidImage;
	sf::Sprite androidSprite;
};
