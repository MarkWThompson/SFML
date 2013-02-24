#include "ConnectionHandler.h"
#include "SharedConstants.h"

ConnectionHandler::ConnectionHandler(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData)
{
	this->serverTransmitter = serverTransmitter;
	this->playerNetworkData = playerNetworkData;
}

ConnectionHandler::~ConnectionHandler()
{
	serverTransmitter = NULL;
	playerNetworkData = NULL;
}

void ConnectionHandler::ValidateConnection(sf::IPAddress connectionAddress, unsigned int port)
{
	// Stores what player we've just added
	int playerAdded = 0;

	// Connection validation either approves or denys a connection. If approved the server sends back a packet containing the approval message
	if(AddConnectionsToPlayerData(connectionAddress,playerNetworkData,playerAdded))  //if there is space to add, + some validation in this function
	{
		SendApprovalMessage(connectionAddress,playerAdded,true);
	}
	else
	{
		SendDenialMessage(connectionAddress);
	}
}

void ConnectionHandler::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	sf::Uint8 packetType;
	receivedPacket >> packetType;
	
	if(packetType == CONNECTION_REQUEST_PACKET)
	{
		std::string requestMessage;
		receivedPacket >> requestMessage;

		if(requestMessage == sharedConstants.GetRequestMessage())
		{
			std::cout << std::endl << "Connection requested by : " + connectionAddress.ToString() << std::endl;
			ValidateConnection(connectionAddress, port);
		}
	}

	if(packetType == CONNECTION_VALIDITY_PACKET)
	{
		int playerID;
		sf::Uint32 stateIteration;
		bool connectionActive;
		
		receivedPacket >> playerID >> stateIteration >> connectionActive;
		if(connectionActive && playerNetworkData->playerIPs[playerID] != NULL_IP)
		{
			playerNetworkData->playerIPs[playerID] = connectionAddress;
			playerNetworkData->playerStateIterators[playerID] = stateIteration;

			// Reset the players timeOutClock
			playerNetworkData->playerTimeoutClocks[playerID].Reset();
		}
	}

	if(packetType == DISCONNECTION_PACKET)
	{
		int playerID;
		receivedPacket >> playerID;

		playerNetworkData->playerIPs[playerID] = NULL_IP;
		playerNetworkData->playersActive[playerID] = false;
		playerNetworkData->playerStateIterators[playerID] = 0;
		playerNetworkData->playerTimeoutClocks[playerID].Reset();
		playerNetworkData->numPlayers--;

		std::cout << "Player ID : " << playerID << " has disconnected" << std::endl;
		
	}
}

bool ConnectionHandler::CheckForDuplicateIPs(sf::IPAddress ipToCheck)
{
	for(int i = 0; i < playerNetworkData->playerIPs.size(); i++)
	{
		if(ipToCheck == playerNetworkData->playerIPs[i]) // If this is a duplicate IP
		{
			return true;
		}
	}
	return false;
}

bool ConnectionHandler::AddConnectionsToPlayerData(sf::IPAddress connectionAddress, PlayerNetworkData* playerNetworkData, int &playerAdded)
{
	for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
	{
		if(CheckForDuplicateIPs(connectionAddress) == false) // If we aren't trying to connect from the same machine
		{
			if(playerNetworkData->playerIPs[i] == NULL_IP)
			{
				if(connectionAddress.IsValid())
				{
					playerNetworkData->playerIPs[i] = connectionAddress;
					playerNetworkData->playersActive[i] = true;
					std::cout << "New player : " << playerNetworkData->playerIPs[i].ToString() << " connected in slot " << i << "." << std::endl;
					playerAdded = i;
					return true;
				}
			}
		}
		else
		{
			SendApprovalMessage(connectionAddress,playerAdded,false);
			break;
		}
	}

	// All the IPs have been tried
	std::cout << "Player : " << connectionAddress << " failed to connect.";
	return false;
}

void ConnectionHandler::Update()
{
	CheckForTimeouts();
}


void ConnectionHandler::SendApprovalMessage(sf::IPAddress &connectionAddress, int &playerAdded, bool shouldResetTimer)
{
	// Send approval response
		ConnectionResponsePacket responsePacket;
		responsePacket.PackData(sharedConstants.START_STATE, sharedConstants.GetAcceptMessage(), playerAdded, playerNetworkData->MAX_PLAYERS, 0, responsePacket);
		sf::Sleep(0.1f);
		
		if(shouldResetTimer == true)
		{
			//reset the players timeOutClock
			playerNetworkData->playerTimeoutClocks[playerAdded].Reset();
		}

		serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);
		std::cout << std::endl << "Sent Approval Packet To : " + connectionAddress.ToString() << std::endl;
}

void ConnectionHandler::SendDenialMessage(sf::IPAddress &connectionAddress)
{
	// Send deinal message
	ConnectionResponsePacket responsePacket;
	responsePacket.PackData(sharedConstants.START_STATE, sharedConstants.GetRejectMessage(), -1, 1, 0, responsePacket);

	sf::Sleep(0.1f);
	serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);
	std::cout << std::endl << "Rejected connection at : " + connectionAddress.ToString()  << std::endl;
}

void ConnectionHandler::CheckForTimeouts()
{
	// Check for player timeouts
	for(int i = 0; i < playerNetworkData->playerIPs.size(); i++)
	{
		if((playerNetworkData->playerTimeoutClocks[i].GetElapsedTime() > sharedConstants.GetTimeout()) && (playerNetworkData->playerIPs[i] != NULL_IP))
		{
			playerNetworkData->playerIPs[i] = NULL_IP;
			playerNetworkData->playersActive[i] = false;
			playerNetworkData->playerStateIterators[i] = 0;
			playerNetworkData->playerTimeoutClocks[i].Reset();
			playerNetworkData->numPlayers--;

			std::cout << "Player ID : " << i << " has timed out! Dropping player." << std::endl;
		}
	}
}
