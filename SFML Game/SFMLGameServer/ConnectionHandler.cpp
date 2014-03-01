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

void ConnectionHandler::ValidateConnection(sf::IPAddress connectionAddress, unsigned int port, std::string playerName)
{
	// Stores what player we've just added
	int playerAdded = -1;

	// Connection validation either approves or denys a connection. If approved the server sends back a packet containing the approval message
	if(AddConnectionsToPlayerData(connectionAddress, playerNetworkData, playerAdded))  //if there is space to add, + some validation in this function
	{
		SendApprovalMessage(connectionAddress, playerAdded, true, playerName);
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
		std::string playerName;
		receivedPacket >> playerName;

		if(requestMessage == sharedConstants.GetRequestMessage())
		{
			std::cout << std::endl << "Connection requested by : " + playerName << " at address : " + connectionAddress.ToString() << std::endl;
			ValidateConnection(connectionAddress, port, playerName);
		}
	}

	if(packetType == DISCONNECTION_PACKET)
	{
		int playerID;
		receivedPacket >> playerID;

		playerNetworkData->FreeSlot(playerID);

		std::cout << "Player ID : " << playerID << " has disconnected" << std::endl;
	}	
}

bool ConnectionHandler::CheckForDuplicateIPs(sf::IPAddress ipToCheck)
{
	for(size_t i = 0; i < playerNetworkData->playerIPs.size(); i++)
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
	int freeSlot = -1;

	if(CheckForDuplicateIPs(connectionAddress) == false) // If we aren't trying to connect from the same machine
	{
		if(connectionAddress.IsValid())
		{
			// Find the nearest free slot
			for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
			{
				if(playerNetworkData->playerIPs[i] == NULL_IP)
				{
					freeSlot = i;
					break;
				}
			}

			playerNetworkData->playerIPs[freeSlot] = connectionAddress;
			playerNetworkData->playersActive[freeSlot] = true;
			std::cout << "New player : " << playerNetworkData->playerIPs[freeSlot].ToString() << " connected in slot " << freeSlot << "." << std::endl;
			playerAdded = freeSlot;
			return true;
		}
	}
	else
	{
		// Retrieve the players ID
		for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
		{
			if(playerNetworkData->playerIPs[i] == connectionAddress)
			{
				playerAdded = i;
				break;
			}
		}

		// Send another approval packet
		std::cout << "Resending approval packet to client : " << connectionAddress.ToString() << std::endl;
		return true;
	}

	// All the IPs have been tried
	std::cout << "Player : " << connectionAddress << " failed to connect due to no free slots.";
	return false;
}

void ConnectionHandler::Update()
{
	CheckForTimeouts();
}

void ConnectionHandler::SendApprovalMessage(sf::IPAddress &connectionAddress, int &playerAdded, bool shouldResetTimer, std::string playerName)
{
	// Send approval response
	ConnectionResponsePacket responsePacket;
	responsePacket.PackData(sharedConstants.LOBBY_STATE, sharedConstants.GetAcceptMessage(), playerAdded, playerNetworkData->MAX_PLAYERS, levelFilepath, 0, responsePacket);
	sf::Sleep(0.1f);

	if(shouldResetTimer == true)
	{
		//reset the players timeOutClock
		playerNetworkData->playerTimeoutClocks[playerAdded].Reset();
	}

	serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);
	std::cout << std::endl << "Sent Approval Packet To : " + playerName << " at address : " + connectionAddress.ToString() << std::endl;

	playerNetworkData->playerNames[playerAdded] = playerName;

	playerNetworkData->playersToSetup[playerAdded] = true;
}

void ConnectionHandler::SendDenialMessage(sf::IPAddress &connectionAddress)
{
	// Send deinal message
	ConnectionResponsePacket responsePacket;
	responsePacket.PackData(sharedConstants.LOBBY_STATE, sharedConstants.GetRejectMessage(), -1, 1, "", 0, responsePacket);

	sf::Sleep(0.1f);
	serverTransmitter->SendUDP(sharedConstants.GetServerTransmitPort(), connectionAddress, responsePacket);
	std::cout << std::endl << "Rejected connection at : " + connectionAddress.ToString()  << std::endl;
}

void ConnectionHandler::CheckForTimeouts()
{
	// Check for player timeouts
	for(size_t i = 0; i < playerNetworkData->playerIPs.size(); i++)
	{
		if((playerNetworkData->playerTimeoutClocks[i].GetElapsedTime() > sharedConstants.GetTimeout()) && (playerNetworkData->playerIPs[i] != NULL_IP))
		{
			playerNetworkData->FreeSlot(i);
			std::cout << "Player ID : " << i << " has timed out! Dropping player." << std::endl;
		}
	}
}

void ConnectionHandler::SetLevelFilepath(std::string levelFilepath)
{
	this->levelFilepath = levelFilepath;
}
