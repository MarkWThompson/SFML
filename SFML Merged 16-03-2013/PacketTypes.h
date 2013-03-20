#pragma once
#include <SFML\Network.hpp>
#include "SharedConstants.h"
#include <iostream>

// Packet identifiers
#define CONNECTION_REQUEST_PACKET 0
#define CONNECTION_RESPONSE_PACKET 1
#define CONNECTION_VALIDITY_PACKET 2
#define PLAYER_POSITIONS_PACKET 3
#define PLAYER_INPUT_PACKET 4
#define DISCONNECTION_PACKET 5
#define PROJECTILE_PACKET 6
#define PROJECTILE_DEATH_PACKET 7
#define PLAYER_DATA_PACKET 8
#define SCORE_REQUEST_PACKET 9
#define SCORE_RESPONSE_PACKET 10

// Packet overloads
//sf::Packet & operator << (sf::Packet & Packet, sf::Vector2f& inputVector2f);
sf::Packet & operator >> (sf::Packet & Packet, sf::Vector2f& outputVector2f);


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

class PlayerDataPacket : public sf::Packet
{
public:
	PlayerDataPacket();
	void PackData(sf::Uint8 routingTag, sf::Uint8 vectorSize, std::vector<std::string> playerNames, std::vector<sf::Int16> currentPlayerHealth, sf::Packet &packetInstance);
};

class DisconnectionPacket : public sf::Packet
{
public:
	DisconnectionPacket();
	void PackData(sf::Uint8 routingTag, int playerID, sf::Packet &packetInstance);
};


// Note : At 60fps, the stateIterator will not overflow for over 9000 (not even kidding, it's 9942 hours) hours. That's over a year of running. You can even double that by using unsigned if you like, you know, for them really bitter grudgematch games that take over 2 years.
class ConnectionValidityPacket : public sf::Packet
{
public:
	ConnectionValidityPacket();
	void PackData(sf::Uint8 routingTag, int playerID, sf::Uint32 stateIteration, bool connectionActive, sf::Packet &packetInstance);
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

class ScoreRequestPacket : public sf::Packet
{
public:
	ScoreRequestPacket();
	void PackData(sf::Uint8 routingTag, sf::Packet &packetInstance);
};

class ScoreResponsePacket : public sf::Packet
{
public:
	ScoreResponsePacket();
	//ScoreVectorSize should really be 8, or maxPlayers
	void PackData(sf::Uint8 routingTag, sf::Uint8 scoreVectorSize, std::vector<int> scoreVector, sf::Packet &packetInstance);
};