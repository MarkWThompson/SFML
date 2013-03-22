#pragma once

#include <SFML\Network.hpp>
#include "ConnectionHandler.h"
#include "GameLogic.h"

class ServerRouter
{
public:
	/** Constructor - module initialiser. */
	ServerRouter(ConnectionHandler* connectionHandler, GameLogic* gameLogic);
	
	/** Default destructor. */
	~ServerRouter();

	/** Sends packet to target module based on routingTag. */
	void RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port);

private:
	// Modules
	ConnectionHandler* connectionHandler;
	GameLogic* gameLogic;
};
