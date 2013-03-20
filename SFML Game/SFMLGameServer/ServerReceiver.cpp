#include "ServerReceiver.h"

ServerReceiver::ServerReceiver(ServerRouter* serverRouter)
{
	this->serverRouter = serverRouter;

	// Bind it to the specified port
	if(!receiver.Bind(sharedConstants.GetServerReceivePort()))
	{
        std::cout << "ServerReceiver() error: failed to bind socket to specified port." << std::endl;
	}

	receiver.SetBlocking(false);
}

ServerReceiver::~ServerReceiver()
{
	receiver.Close();
}

void ServerReceiver::ReceiveUDP()
{
	if(receiver.IsValid())
	{
		if(receiver.Receive(receivePacket, receiveAddress, receivePort) == sf::Socket::Done)
		{
			//std::cout << "Server received packet from IP(" << receiveAddress << "), Port(" << receivePort << ")." << std::endl;

			serverRouter->RouteData(receivePacket, receiveAddress, receivePort);
		}
	}
}
