#include <iostream>
#include <SFML\Network.hpp>
#include "ServerTransmitter.h"
#include "ServerReceiver.h"
#include "ServerRouter.h"
#include "ConnectionHandler.h"
#include "..\SharedConstants.h"

int main()
{
	// PIPES
	ServerTransmitter serverTransmitter;
	ServerReceiver serverReceiver;
	ServerRouter* serverRouter;

	// Modules
	ConnectionHandler* connectionHandler = new ConnectionHandler(serverTransmitter);

	std::cout << "Loading Skynet 2.0" << std::endl;

	std::cout << "..." << std::endl;

	// Update router with modules
	serverRouter = new ServerRouter(*connectionHandler);

	std::cout << "Skynet 2.0 initialised, awaiting connections... " << std::endl;

	while(true)
	{
		serverReceiver.ReceiveUDP(sharedConstants.GetServerReceivePort(), *serverRouter, true);
	}

	return EXIT_SUCCESS;
}