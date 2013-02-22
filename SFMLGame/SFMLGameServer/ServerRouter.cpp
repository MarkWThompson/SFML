#include "ServerRouter.h"
#include "SharedConstants.h"

ServerRouter::ServerRouter(ConnectionHandler* connectionHandler, GameLogic* gameLogic)
{
	this->connectionHandler = connectionHandler;
	this->gameLogic = gameLogic;
}

ServerRouter::~ServerRouter()
{
	delete gameLogic;
	gameLogic = NULL;

	delete connectionHandler;
	connectionHandler = NULL;
}

void ServerRouter::RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 routingTag;

	// Unpack routing tag
	packetToRoute >> routingTag;

	// std::cout << "Server router sent packet to module with routingTag(" << (int)routingTag << ")." << std::endl;

	if(routingTag == sharedConstants.CONNECT_MODULE)
	{
		// ClientController can receive
		// 1. Connection Packets, Uint8 packetType, string connectionMessage
		connectionHandler->ReceiveData(packetToRoute, connectionAddress, port);
	}
	else if(routingTag == sharedConstants.GAME_MODULE)
	{
		gameLogic->ReceiveData(packetToRoute, connectionAddress, port);
	}
	else if(routingTag == sharedConstants.CHAT_MODULE)
	{

	}
	else
	{
		std::cout << "ServerRouter::RouteData() error: Unknown routingTag." << std::endl;
	}
}
