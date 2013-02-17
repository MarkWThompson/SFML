#include <SFML\Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"
#include "ServerReceiver.h"
#include "ServerRouter.h"
#include "ConnectionHandler.h"
#include "..\SharedConstants.h"

int main()
{
	std::cout << "Loading Skynet 2.0" << std::endl;
	
	// This order is quite important
	ServerTransmitter serverTransmitter;

	ConnectionHandler* connectionHandler = new ConnectionHandler(serverTransmitter);
	GameLogic* gameLogic = new GameLogic(serverTransmitter);

	ServerRouter* serverRouter = new ServerRouter(*connectionHandler, *gameLogic);
	ServerReceiver serverReceiver(false, *serverRouter);

	std::cout << "Skynet 2.0 initialised and now running..." << std::endl;

	while(true)
	{
		serverReceiver.ReceiveUDP();
		gameLogic->Update();
	}

	return EXIT_SUCCESS;
}