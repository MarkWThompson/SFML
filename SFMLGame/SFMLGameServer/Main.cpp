#include <SFML\Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"
#include "ServerReceiver.h"
#include "ServerRouter.h"
#include "ConnectionHandler.h"
#include "SharedConstants.h"
#include "PlayerNetworkData.h"

int main()
{
	std::cout << "Loading Skynet 2.0" << std::endl;
	
	PlayerNetworkData* playerNetworkData = new PlayerNetworkData();

	// This order is quite important
	ServerTransmitter* serverTransmitter = new ServerTransmitter();

	ConnectionHandler* connectionHandler = new ConnectionHandler(serverTransmitter, playerNetworkData);
	GameLogic* gameLogic = new GameLogic(serverTransmitter, playerNetworkData);

	ServerRouter* serverRouter = new ServerRouter(connectionHandler, gameLogic);
	ServerReceiver* serverReceiver = new ServerReceiver(false, serverRouter);

	std::cout << "Skynet 2.0 initialised and now running..." << std::endl;

	while(true)
	{
		serverReceiver->ReceiveUDP();
		connectionHandler->Update();
		gameLogic->Update();
	}

	// Cleanup
	delete serverTransmitter;
	delete connectionHandler;
	delete gameLogic;
	delete serverRouter;
	delete serverReceiver;
	delete playerNetworkData;

	playerNetworkData = NULL;
	serverTransmitter = NULL;
	connectionHandler = NULL;
	gameLogic = NULL;
	serverRouter = NULL;
	serverReceiver = NULL;

	return EXIT_SUCCESS;
}
