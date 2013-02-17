#include "ServerReceiver.h"

ServerReceiver::ServerReceiver(bool isBlocking, ServerRouter &serverRouter)
{
	this->serverRouter = &serverRouter;

	// Bind it to the specified port
	if(!receiver.Bind(sharedConstants.GetServerReceivePort()))
	{
        std::cout << "ServerReceiver(bool isBlocking, ServerRouter &serverRouter) error: failed to bind socket to specified port." << std::endl;
	}

	if(isBlocking == true)
	{
		receiver.SetBlocking(true);
	}
	else
	{
		receiver.SetBlocking(false);
	}
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
			serverRouter->RouteData(receivePacket, receiveAddress, receivePort);
		}
	}
}
