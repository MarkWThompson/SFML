#pragma once

#include <SFML/Network.hpp>
#include <iostream>

class ClientTransmitter
{
public:
	/** Default constructor. */
	ClientTransmitter();

	/** Default destructor. */
	~ClientTransmitter();
	
	/** 
	 * Sends a packet via the specified port to the target address.
	 * @param port The port which the packet is sent through.
	 * @param targetAddress The IP address which the packet should be sent to.
	 * @param sendPacket The packet which should be sent.
	 */
	void SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket);
};
