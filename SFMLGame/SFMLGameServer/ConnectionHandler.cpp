#include "ConnectionHandler.h"
#include "..\SharedConstants.h"
// FormatTag = 0 is a ConnectionPacket, Uint8 formatTag, string connectionMessage

ConnectionHandler::ConnectionHandler()
{
	Init();
}

ConnectionHandler::ConnectionHandler(ServerTransmitter &serverTransmitter)
{
	this->serverTransmitter = &serverTransmitter;
	Init();
}

ConnectionHandler::~ConnectionHandler()
{
}

void ConnectionHandler::Init()
{
	numPlayers = 0;
}

void ConnectionHandler::ValidateConnection(sf::IPAddress connectionAddress, unsigned int port)
{
	// Connection validation either approves or denys a connection. If approved the server sends back a packet containing the approval message
	if(numPlayers < maxPlayers)
	{
		numPlayers++;
		playerIPs.push_back(connectionAddress);
		// Send approval response
		ConnectionResponsePacket responsePacket;
		responsePacket.PackData(0,sharedConstants.GetAcceptMessage(),responsePacket);

		sf::Sleep(0.1f);
		serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);
		std::cout << std::endl << "Accepted connection at : " + connectionAddress.ToString() << std::endl;
	}
	else
	{
		//send deinal message

		ConnectionResponsePacket responsePacket;
		responsePacket.PackData(0,sharedConstants.GetRejectMessage(),responsePacket);

		sf::Sleep(0.1f);
		serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);
		std::cout << std::endl << "Rejected connection at : " + connectionAddress.ToString() + "     : Server full." << std::endl;
	}
}

void ConnectionHandler::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	
	sf::Uint8 packetType;
	receivedPacket >> packetType;
	
	if(packetType == CONNECTION_REQUEST_PACKET)
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

