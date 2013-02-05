#pragma once
#include <SFML\Network.hpp>
#include "ConnectionHandler.h"

class ServerRouter
{
public:
	ServerRouter();
	ServerRouter(ConnectionHandler &connectionHandler); // Parameter should most likely be a vector containing all modules
	~ServerRouter();
	void RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port);

private:
	ConnectionHandler* connectionHandler; //routingTag = 0;

};

