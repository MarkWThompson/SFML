#pragma once
#include <SFML\Network.hpp>
#include "ClientConnector.h"

class ServerRouter
{
public:
	ServerRouter();
	ServerRouter(ClientConnector &clientConnector); // Parameter should most likely be a vector containing all modules
	~ServerRouter();
	void RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port);

private:
	ClientConnector* clientConnector; //routingTag = 0;

};

