#include "ServerTransmitter.h"
#include "SharedConstants.h"

ServerTransmitter::ServerTransmitter()
{
}

ServerTransmitter::~ServerTransmitter()
{
	sender.Close();
}

void ServerTransmitter::SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket)
{
    if(targetAddress.IsValid())
	{
		// Send a message to the server
		if(sender.Send(sendPacket, targetAddress, port) != sf::Socket::Done)
		{
			std::cout << "ServerTransmitter::SendUDP() error: failed to send packet to target." << std::endl;
			return;
		}

		// std::cout << "Message sent to client : " << targetAddress.ToString() << " on port " << port << std::endl;
	}
}
