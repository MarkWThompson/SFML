#include "ClientTransmitter.h"
#include "SharedConstants.h"

ClientTransmitter::ClientTransmitter()
{
}

ClientTransmitter::~ClientTransmitter()
{
	sender.Close();
}

void ClientTransmitter::SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket)
{
	if(targetAddress.IsValid())
	{
		// Send a message to the server
		if(sender.Send(sendPacket, targetAddress, port) != sf::Socket::Done)
		{
			std::cout << "ClientTransmitter::SendUDP() error: failed to send packet to target." << std::endl;
			return;
		}

		//std::cout << std::endl << "Message sent to server : " << targetAddress.ToString() << " on port " << port << std::endl;
	}
}
