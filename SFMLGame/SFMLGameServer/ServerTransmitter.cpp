#include "ServerTransmitter.h"
#include "SharedConstants.h"

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
		std::cout << "ServerTransmitter::SendUDP() error: IP invalid." << std::endl;
	}
	else
	{
		sf::SocketUDP sender;

		// Please leave for now -- needs testing!
		/*
		if(!sender.Bind(sharedConstants.GetServerTransmitPort()))
		{
			std::cout << "ServerTransmitter() error: failed to bind socket to port." << std::endl;
		}
		*/

		// Send a message to the server
		if(sender.Send(sendPacket, targetAddress, port) != sf::Socket::Done)
		{
			std::cout << "ServerTransmitter::SendUDP() error: failed to send packet to target." << std::endl;
			return;
		}

		// std::cout << "Message sent to client : " << targetAddress.ToString() << " on port " << port << std::endl;

		sender.Close();
	}
}
