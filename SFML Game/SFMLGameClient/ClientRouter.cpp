#include "ClientRouter.h"
#include "..\SharedConstants.h"

ClientRouter::ClientRouter()
{
	this->curState = NULL;
}

ClientRouter::ClientRouter(StartState* startState, GameState* gameState)
{
	this->startState = startState; 
	this->gameState = gameState;
}

ClientRouter::~ClientRouter()
{
	//These two do actually have to be before the deletes, I think this is the way you should always do it imo.
	gameModule = NULL;
	connectModule = NULL;

	delete gameModule;
	delete connectModule;

}

void ClientRouter::RouteData(sf::Packet packetToRoute, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 routingTag;

	// Unpack what type of data it is
	packetToRoute >> routingTag;

	std::cout << "Packet routing tag: " << (int)routingTag << std::endl;

	// Unpack the actual message and tag, the give to whereever it needs to go
	

	// Actually send the data
	if(routingTag == sharedConstants.CONNECT_MODULE)
	{
		std::cout << "Client router sent packet to ConnectModule. " << std::endl;
		connectModule->ReceiveData(packetToRoute, connectionAddress, port);
	}
	else if(routingTag == sharedConstants.GAME_MODULE)
	{
		std::cout << "Client router sent packet to GameModule." << std::endl;
		gameModule->ReceiveData(packetToRoute, connectionAddress, port);
	}
	else if(routingTag == sharedConstants.CHAT_MODULE)
	{
		std::cout << "Client router sent packet to ChatModule." << std::endl;
		//chat.receiveData(data,dataTag);
	}
	else
	{
		std::cout << "Client router received packet with an unknown packetType. This packet has not been sent." << std::endl;
	}
}
