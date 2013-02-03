#pragma once

#include <SFML/Network.hpp>
#include <iostream>

class ClientTransmitter
{
public:
	ClientTransmitter();
	~ClientTransmitter();
	
	void SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket);

private:
	sf::Packet sendPacket;

};

