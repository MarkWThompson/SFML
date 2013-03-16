#pragma once

#include <SFML\Network.hpp>
#include <iostream>

class ServerTransmitter
{
public:
	/** Default constructor. */
	ServerTransmitter();

	/** Default destructor. */
	~ServerTransmitter();

	/** Sends packet via the specified port to the target IP address. */
	void SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket);

private:
	sf::SocketUDP sender;

};
