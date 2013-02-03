#pragma once

class ServerTransmitter
{
public:
	ServerTransmitter();
	~ServerTransmitter();
	static void SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket);
};

