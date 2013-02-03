#pragma once

#include <SFML\Network.hpp>
#include <iostream>

// Client connector can take
// A connectionResponse packet , Uint8 formatTag, std::string responseString
class ClientConnector
{
public:
	ClientConnector();
	~ClientConnector();
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);
	
	sf::IPAddress serverIP;
	unsigned short port;
	bool isConnected;
};
