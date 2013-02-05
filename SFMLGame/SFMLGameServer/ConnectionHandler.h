#pragma once
#include <SFML\Network.hpp>
#include "ServerTransmitter.h"
#include "..\PacketTypes.h"
#include <iostream>


class ConnectionHandler
{
public:
	ConnectionHandler();
	ConnectionHandler(ServerTransmitter &serverTransmitter);
	~ConnectionHandler();
	void Init();
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	void ValidateConnection(sf::IPAddress connectionAddress, unsigned int port); // Check if connecting player may connect
	ServerTransmitter* serverTransmitter;

	std::vector<sf::IPAddress> playerIPs;
	static const int maxPlayers = 8;
	int numPlayers;

	//Receive Responders
	void receiveResponse(ConnectionRequestPacket receivedPacket);
};

