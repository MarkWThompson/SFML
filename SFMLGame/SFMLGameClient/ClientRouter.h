#pragma once

#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include "ClientConnector.h"
#include <iostream>

class ClientRouter
{
public:
	ClientRouter();
	ClientRouter(ClientConnector &clientConnector);
	~ClientRouter();

	void RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port);

private:
	ClientConnector *clientConnector; // if more of these are added, remember to update setup

};
