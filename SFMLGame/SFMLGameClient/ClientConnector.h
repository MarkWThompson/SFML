#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include "..\PacketTypes.h"


class ClientConnector
{
public:
	ClientConnector();
	~ClientConnector();
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);
	
	sf::IPAddress serverIP;
	unsigned short port;
	bool isConnected;
	int playerID; //Received in response packet from server
};
