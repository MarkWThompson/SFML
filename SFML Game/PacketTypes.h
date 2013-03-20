#pragma once

#include <SFML\Network.hpp>
#include "SharedConstants.h"
#include <iostream>

// Packet identifiers
#define CONNECTION_REQUEST_PACKET 0
#define CONNECTION_RESPONSE_PACKET 1
#define PLAYER_POSITIONS_PACKET 2
#define PLAYER_INPUT_PACKET 3
#define DISCONNECTION_PACKET 4
#define PROJECTILE_PACKET 5
#define PROJECTILE_DEATH_PACKET 6
#define PLAYER_DATA_PACKET 7

// Packet overloads
//sf::Packet & operator << (sf::Packet & Packet, sf::Vector2f& inputVector2f);
sf::Packet & operator >> (sf::Packet & Packet, sf::Vector2f& outputVector2f);


//*************************************
//
//	CONNECTION
//
//*************************************
class ConnectionRequestPacket : public sf::Packet
{
public:
	ConnectionRequestPacket();
	void PackData(sf::Uint8 routingTag, std::string playerName, sf::Packet &packetInstance);
};

class ConnectionResponsePacket : public sf::Packet
{
public:
	ConnectionResponsePacket();
	void PackData(sf::Uint8 routingTag, std::string responseMessage, int playerID, sf::Uint8 maxPlayers, std::string levelFilepath, sf::Uint32 stateIterator, sf::Packet &packetInstance);
};


//*************************************
//
//	PLAYER
//
//*************************************
class PlayerDataPacket : public sf::Packet
{
public:
	PlayerDataPacket();
	void PackData(sf::Uint8 routingTag, sf::Uint8 vectorSize, std::vector<std::string> playerNames, std::vector<sf::Int16> playerScores, std::vector<sf::Int16> currentPlayerHealth, sf::Packet &packetInstance);
};

class PlayerPositionsPacket : public sf::Packet
{
public:
	PlayerPositionsPacket();
	// playerNum MUST be the size of the array, the array size should really be the amount of players there are.
	void PackData(sf::Uint8 routingTag, sf::Uint8 playerPosVectorSize, std::vector<bool> playersActive, std::vector<sf::Vector2f> playerPositions, sf::Uint32 stateIterator, sf::Packet &packetInstance);
};

class PlayerInputPacket : public sf::Packet
{
public:
	PlayerInputPacket();
	void PackData(sf::Uint8 routingTag, int playerID, bool wDown, bool aDown, bool sDown, bool dDown, bool spaceDown, bool escDown, 
				  bool returnDown, bool lBtnDown, bool rBtnDown, sf::Int16 mouseX, sf::Int16 mouseY, sf::Packet &packetInstance);
};


//*************************************
//
//	CLIENT
//
//*************************************
class DisconnectionPacket : public sf::Packet
{
public:
	DisconnectionPacket();
	void PackData(sf::Uint8 routingTag, int playerID, sf::Packet &packetInstance);
};


//*************************************
//
//	PROJECTILE
//
//*************************************
class ProjectilePacket : public sf::Packet
{
public:
	ProjectilePacket();
	void PackData(sf::Uint8 routingTag, sf::Uint32 bulletID, sf::Vector2f bulletPosition, sf::Vector2f bulletVelocity, sf::Uint32 stateIterator, sf::Packet &packetInstance);
};

class ProjectileDeathPacket : public sf::Packet
{
public:
	ProjectileDeathPacket();
	void PackData(sf::Uint8 routingTag, sf::Uint32 bulletID, sf::Vector2f deathPosition, sf::Uint32 stateIterator, sf::Packet &packetInstance);
};
