#include "ClientConnector.h"
#include "..\SharedConstants.h"

ClientConnector::ClientConnector()
{
	isConnected = false;
}

ClientConnector::~ClientConnector()
{
}

void ClientConnector::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	//A connectionResponse packet , Uint8 formatTag, std::string responseString, Uint8 playerNumber
	//PACKET MUST HAVE A FORMATTAG AS THE FIRST ENTRY, TO FACILITATE MULTIPLE PACKET TYPES
	sf::Uint8 packetType;
	receivedPacket >> packetType;

	// The format tag as a magic number will probably need to be discussed
	if(packetType == CONNECTION_RESPONSE_PACKET) // Connection responses
	{
		std::string requestResponse;
		receivedPacket >> requestResponse;

		if(requestResponse == sharedConstants.GetAcceptMessage())
		{
			std::cout << std::endl << "Connection approved by server : " + connectionAddress.ToString() << std::endl;

			//unpack this players number
			receivedPacket >> playerID;

			serverIP = connectionAddress;
			this->port = port;
			isConnected = true;

			std::cout << "Player ID : " << playerID;
		}
		else if(requestResponse == sharedConstants.GetRejectMessage())
		{
			std::cout << std::endl << "Connection denied by server : " + connectionAddress.ToString() << std::endl;

			serverIP = connectionAddress;
			this->port = port;
			isConnected = false;
		}
	}
}
