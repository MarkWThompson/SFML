#include "ClientReceiver.h"

ClientReceiver::ClientReceiver(bool isBlocking)
{
	if(!receiver.Bind(sharedConstants.GetClientReceivePort()))
	{
		std::cout << "ClientReceiver() error: failed to bind socket to specified port." << std::endl;
	}

	if(isBlocking == false)
	{
		receiver.SetBlocking(false);
	}
	else
	{
		receiver.SetBlocking(true);
	}
}

ClientReceiver::~ClientReceiver()
{
	receiver.Close();
}

void ClientReceiver::ReceiveUDP(State* curState)
{
	if(receiver.IsValid())
	{
		if(receiver.Receive(receivedPacket, receiveAddress, receivePort) == sf::Socket::Done)
		{
			//std::cout << std::endl << "Message receieved from : " + receiveAddress.ToString() << " on port : " << receivePort << std::endl;
			sf::Uint8 routingTag;

			// Unpack what type of data it is
			receivedPacket >> routingTag;

			//std::cout << "Packet routing tag: " << (int)routingTag << std::endl;

			/* This is validation, checks that the packet is right for the state, just in case.
			 * Probably will be useful in some other capacity.
			 * The reason the routingTag is still in is basically so packets can be reused both ways, although this validation is nice too.
			 */
			if(routingTag == curState->GetTarget())
			{
				curState->ReceiveData(receivedPacket, receiveAddress, receivePort);
			}
		}
	}
	else
	{
		std::cout << "ClientReceiver::ReceiveUDP() error: socket invalid." << std::endl;
	}
}
