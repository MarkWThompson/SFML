#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include "State.h"

class ClientReceiver
{
public:
	ClientReceiver();

	~ClientReceiver();

	/** 
	 * Listens for packets sent to the client. Once obtained, sends the packet to the target state.
	 * @param curState The active FSM state which the packet should be routed to for processing.
	 */
	void ReceiveUDP(State* curState);

private:
	/** The socket used to listen for packets. */
	sf::SocketUDP receiver;

	// Received packet storage
	sf::Packet receivedPacket;		///< Stores the most recently received packet.
	sf::IPAddress receiveAddress;	///< Stores the address which the most recent packet was sent from.
	unsigned short receivePort;		///< Stores the port which the most recent packet was sent from.
};
