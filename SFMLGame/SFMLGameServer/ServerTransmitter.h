#pragma once

class ServerTransmitter
{
public:
	/** Default constructor. */
	ServerTransmitter();

	/** Default destructor. */
	~ServerTransmitter();

	/** Sends packet via the specified port to the target IP address. */
	static void SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket);
};
