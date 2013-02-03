#include "ServerRouter.h"
#include "..\SharedConstants.h"

ServerRouter::ServerRouter()
{
}

ServerRouter::ServerRouter(ClientConnector &clientConnector)
{
	this->clientConnector = &clientConnector;
}

ServerRouter::~ServerRouter()
{
}

//routing the data
void ServerRouter::RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 routingTag;

	// Unpack routing tag
	packetToRoute >> routingTag;

	if(routingTag == sharedConstants.CONNECT_MODULE)
	{
		// ClientController can receive
		// 1. Connection Packets, Uint8 formatTag, string connectionMessage
		clientConnector->ReceiveData(packetToRoute, connectionAddress, port);
	}
	else if(routingTag == sharedConstants.GAME_MODULE)
	{
		// game->ReceiveData(packetToRoute, connectionAddress, port);
	}
	else if(routingTag == sharedConstants.CHAT_MODULE)
	{
		// chat->ReceiveData(packetToRoute, connectionAddress, port);
	}
}
