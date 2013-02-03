#include "ClientReceiver.h"

ClientReceiver::ClientReceiver()
{
}

ClientReceiver::~ClientReceiver()
{
}

void ClientReceiver::ReceiveUDP(unsigned short port, ClientRouter &networkRouter, bool blocking)
{
    // Create a UDP socket for communicating with clients
    sf::SocketUDP Receiver;

    // Bind it to the specified port
    if(!Receiver.Bind(port))
	{
        return;
	}

	if(blocking == true)
	{
		Receiver.SetBlocking(true);
	}
	else if(blocking == false)
	{
		Receiver.SetBlocking(false);
	}

	// Storage for any packets that are received
	sf::Packet receivePacket;
	sf::IPAddress receiveAddress;
	unsigned short receivePort;

    if (Receiver.Receive(receivePacket, receiveAddress, receivePort) != sf::Socket::Done)
	{
        return;
	}
	
	std::cout << std::endl << "Message receieved from : " + receiveAddress.ToString() << std::endl;
	
	networkRouter.RouteData(receivePacket, receiveAddress, receivePort);
    
	// Close the socket
    Receiver.Close();
}
