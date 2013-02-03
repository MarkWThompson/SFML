#pragma once
#include <SFML\Network.hpp>
#include "ServerTransmitter.h"
#include <iostream>

//FormatTag = 0 is a ConnectionPacket, Uint8 formatTag, string connectionMessage

class ClientConnector
{
public:
	ClientConnector();
	ClientConnector(ServerTransmitter &serverTransmitter);
	~ClientConnector();
	void Init();
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	void ValidateConnection(sf::IPAddress connectionAddress, unsigned int port); // Check if connecting player may connect
	ServerTransmitter* serverTransmitter;

	std::vector<sf::IPAddress> playerIPs;
	static const int maxPlayers = 8;
	int numPlayers;
};

