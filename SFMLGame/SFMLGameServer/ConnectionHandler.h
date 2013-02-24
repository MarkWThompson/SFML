#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include "ServerTransmitter.h"
#include "PacketTypes.h"
#include "PlayerNetworkData.h"

class ConnectionHandler
{
public:
	/** Constructor. */
	ConnectionHandler(ServerTransmitter* serverTransmitter, PlayerNetworkData* playerNetworkData);

	/** Default destructor. */
	~ConnectionHandler();

	/** Main loop. */
	void Update();

	/** Listens for packets and processes valid packet content. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	/** Checks if connecting player can connect. */
	void ValidateConnection(sf::IPAddress connectionAddress, unsigned int port);
	
	/** Returns true if the specified IP is stored in the player network data, otherwise returns false. */
	bool CheckForDuplicateIPs(sf::IPAddress ipToCheck);

	/** Deals with connecting players, placing them in the right slot. Returns true if a player successfully connects, otherwise returns false. */
	bool AddConnectionsToPlayerData(sf::IPAddress connectionAddress,PlayerNetworkData* playerNetworkData, int &playerAdded);
	
	/** Checks that players are still sending validity packets. */
	void CheckForTimeouts();

	void SendApprovalMessage(sf::IPAddress &connectionAddress, int &playerAdded, bool shouldResetTimer);
	void SendDenialMessage(sf::IPAddress &connectionAddress);
	void AddPlayer(int PlayerID);

	/** Packet output stream. */
	ServerTransmitter* serverTransmitter;

	/** Connected player data. */
	PlayerNetworkData* playerNetworkData;
};
