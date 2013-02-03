#include "ClientRouter.h"
#include "..\SharedConstants.h"

ClientRouter::ClientRouter()
{
}

ClientRouter::ClientRouter(ClientConnector &clientConnector)
{
	this->clientConnector = &clientConnector; 
}

ClientRouter::~ClientRouter()
{
}

void ClientRouter::RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 routingTag;

	// Unpack what type of data it is
	packetToRoute >> routingTag;

	// Unpack the actual message and tag, the give to whereever it needs to go
	
	// Actually send the data
	if(routingTag == sharedConstants.CONNECT_MODULE)
	{
		clientConnector->ReceiveData(packetToRoute, connectionAddress, port);
	}
	else if(routingTag == sharedConstants.GAME_MODULE)
	{
		//main/game.receiveData(data,dataTag)
	}
	else if(routingTag == sharedConstants.CHAT_MODULE)
	{
		//chat.receiveData(data,dataTag);
	}
}
