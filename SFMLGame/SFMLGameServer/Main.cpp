#include <iostream>
#include <SFML\Network.hpp>
#include "ServerTransmitter.h"
#include "ServerReceiver.h"
#include "ServerRouter.h"
#include "ClientConnector.h"
#include "..\SharedConstants.h"

int main()
{
	// PIPES
	ServerTransmitter serverTransmitter;
	ServerReceiver serverReceiver;
	ServerRouter* serverRouter;

	// Modules
	ClientConnector* clientConnector = new ClientConnector(serverTransmitter);

	std::cout << "Loading Skynet 2.0" << std::endl;

	std::cout << "..." << std::endl;

	// Update router with modules
	serverRouter = new ServerRouter(*clientConnector);

	std::cout << "Skynet 2.0 initialised, awaiting connections... " << std::endl;

	while(true)
	{
		serverReceiver.ReceiveUDP(sharedConstants.GetServerReceivePort(), *serverRouter, true);
	}

	return EXIT_SUCCESS;
}