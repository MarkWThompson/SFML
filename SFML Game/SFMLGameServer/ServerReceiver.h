#pragma once

#include "ServerRouter.h"

class ServerReceiver
{
public:
	/** Constructor - initialise socket. */
	ServerReceiver(ServerRouter* serverRouter);

	/** Default destructor. */
	~ServerReceiver();

	/** Listens for and receives packets. If valid, will route the packet to target module. */
	void ReceiveUDP();

private:
	/** Pointer to the server router instance. */
	ServerRouter *serverRouter;

	/** Socket for receiving packets. */
    sf::SocketUDP receiver;

	// Storage for received packet data
	sf::Packet receivePacket;
	sf::IPAddress receiveAddress;
	unsigned short receivePort;
};
