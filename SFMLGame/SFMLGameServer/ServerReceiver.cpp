#include "ServerReceiver.h"

ServerReceiver::ServerReceiver()
{
}

ServerReceiver::~ServerReceiver()
{
}

void ServerReceiver::ReceiveUDP(unsigned short port, ServerRouter &networkRouter, bool blocking)
{
    // Create a UDP socket for communicating with clients
    sf::SocketUDP receiver;

    // Bind it to the specified port
    if (!receiver.Bind(port))
	{
        return;
	}

	if(blocking == true)
	{
		receiver.SetBlocking(true);
	}
	else if(blocking == false)
	{
		receiver.SetBlocking(false);
	}

	// Storage for received packet data
	sf::Packet receivePacket;
	sf::IPAddress receiveAddress;
	unsigned short receivePort;

    if(receiver.Receive(receivePacket, receiveAddress, receivePort) != sf::Socket::Done)
	{
        return;
	}

	networkRouter.RouteData(receivePacket, receiveAddress, receivePort);

    // Close the socket when we're done
    receiver.Close();
}
