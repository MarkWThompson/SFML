#include "ClientTransmitter.h"
#include "SharedConstants.h"

ClientTransmitter::ClientTransmitter()
{
	// Please leave this for now!
	/*
	if(!sender.Bind(sharedConstants.GetClientTransmitPort()))
	{
		std::cout << "ClientTransmitter() error: failed to bind socket to port." << std::endl;
	}
	*/
}

ClientTransmitter::~ClientTransmitter()
{
	sender.Close();
}

void ClientTransmitter::SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket)
{
	if(targetAddress.IsValid() == false)
	{
		std::cout << "ClientTransmitter::SendUDP() error: IP invalid." << std::endl;
	}
	else
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
