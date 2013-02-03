#include "ClientConnector.h"
#include "..\SharedConstants.h"
// FormatTag = 0 is a ConnectionPacket, Uint8 formatTag, string connectionMessage

ClientConnector::ClientConnector()
{
	Init();
}

ClientConnector::ClientConnector(ServerTransmitter &serverTransmitter)
{
	this->serverTransmitter = &serverTransmitter;
	Init();
}

ClientConnector::~ClientConnector()
{
}

void ClientConnector::Init()
{
	numPlayers = 0;
}

void ClientConnector::ValidateConnection(sf::IPAddress connectionAddress, unsigned int port)
{
	// Connection validation either approves or denys a connection. If approved the server sends back a packet containing the approval message + 
	if(numPlayers < maxPlayers)
	{
		numPlayers++;
		playerIPs.push_back(connectionAddress);
		// Send approval response

		// Pack a connection resonse packet, uint8 routingTag, uint8 formatTag, std::string responseMessage
		sf::Packet responsePacket;
		sf::Uint8 routingTag = 0;
		sf::Uint8 formatTag = 0;

		responsePacket << routingTag << formatTag << sharedConstants.GetAcceptMessage();

		sf::Sleep(0.1f);
		serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);

		std::cout << std::endl << "Accepted connection at : " + connectionAddress.ToString() << std::endl;
	}
	else
	{
		//send deinal message

		//Pack a connection resonse packet, uint8 routingTag, uint8 formatTag, std::string responseMessage
		sf::Packet responsePacket;
		sf::Uint8 routingTag = 0;
		sf::Uint8 formatTag = 0;

		responsePacket << routingTag << formatTag << sharedConstants.GetRejectMessage();

		sf::Sleep(0.1f);
		serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);
		std::cout << std::endl << "Rejected connection at : " + connectionAddress.ToString() + "     : Server full." << std::endl;
	}
}

void ClientConnector::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	// PACKET MUST HAVE A FORMATTAG AS THE FIRST ENTRY, TO FACILITATE MULTIPLE PACKET TYPES
	sf::Uint8 formatTag;
	receivedPacket >> formatTag;
	
	// If it is a connection request packet, which is Uint8 formatTag, string requestMessage
	if(formatTag == 0)
	{
		std::string requestMessage;

		receivedPacket >> requestMessage;

		if(requestMessage == sharedConstants.GetRequestMessage())
		{
			std::cout << std::endl << "Connection requested by : " + connectionAddress.ToString() << std::endl;
			ValidateConnection(connectionAddress, port);
		}
	}
}
