#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"
#include "PacketTypes.h"
#include "PlayerNetworkData.h"

class ConnectionHandler
{
public:
	/** Deafult donstructor. */
	ConnectionHandler(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData);

	/** Default destructor. */
	~ConnectionHandler();

	/** Main loop. */
	void Update();

	/** Sets the levelFilepath variable. */
	void SetLevelFilepath(std::string levelFilepath);

	/** Listens for packets and processes valid packet content. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	/** Checks if connecting player can connect. */
	void ValidateConnection(sf::IPAddress connectionAddress, unsigned int port, std::string playerName);
	
	/** Returns true if the specified IP is stored in the player network data, otherwise returns false. */
	bool CheckForDuplicateIPs(sf::IPAddress ipToCheck);

	/** Deals with connecting players, placing them in the right slot. Returns true if a player successfully connects, otherwise returns false. */
	bool AddConnectionsToPlayerData(sf::IPAddress connectionAddress,PlayerNetworkData* playerNetworkData, int &playerAdded);
	
	/** Checks that players are still sending validity packets. */
	void CheckForTimeouts();

	// Connection responses
	void SendApprovalMessage(sf::IPAddress &connectionAddress, int &playerAdded, bool shouldResetTimer, std::string playerName);
	void SendDenialMessage(sf::IPAddress &connectionAddress);

	/** Adds player to the server PlayerNetworkData. */
	void AddPlayer(int PlayerID);

	/** Stores the level which the server is hosting. */
	std::string levelFilepath;

	/** Packet output stream. */
	ServerTransmitter* serverTransmitter;

	/** Connected player data. */
	PlayerNetworkData* playerNetworkData;
};
