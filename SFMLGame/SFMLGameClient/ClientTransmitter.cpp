#include "ClientTransmitter.h"

ClientTransmitter::ClientTransmitter()
{
}

ClientTransmitter::~ClientTransmitter()
{
}

void ClientTransmitter::SendUDP(unsigned short port, sf::IPAddress targetAddress, sf::Packet sendPacket)
{
	if(targetAddress.IsValid() == false)
	{
		std::cout << "IP Invalid" << std::endl;
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

		std::cout << std::endl << "Message sent to server : " << targetAddress.ToString() << " on port " << port << std::endl;

		// Close the socket
		sender.Close();
	}
}
