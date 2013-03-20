#pragma once

#include <SFML\Network.hpp>
#include "State.h"
#include <iostream>

class ClientRouter
{
public:
	ClientRouter();
	ClientRouter(State *currentState);
	~ClientRouter();

	void RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port);

private:
	State *curState;

};
