#include <SFML/Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"

ServerTransmitter::ServerTransmitter()
{
}

ServerTransmitter::~ServerTransmitter()
{
}

void ServerTransmitter::SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket)
{
    if(targetAddress.IsValid() == false)
	{
		std::cout << "IP invalid." << std::endl;
	}
	else
	{
		// Create a UDP socket for communicating with server
		sf::SocketUDP sender;

		// Send a message to the server
		if (sender.Send(sendPacket, targetAddress, port) != sf::Socket::Done)
		{
			return;
		}

		std::cout << "Message sent to client : " << targetAddress.ToString() << " on port " << port << std::endl;
		
		// Close the socket
		sender.Close();
	}
}
