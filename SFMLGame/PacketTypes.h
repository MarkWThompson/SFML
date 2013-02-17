#pragma once
#include <SFML\Network\Packet.hpp>
#include "SharedConstants.h"

#define CONNECTION_REQUEST_PACKET 0
#define CONNECTION_RESPONSE_PACKET 1
#define CONNECTION_VALIDITY_PACKET 2
#define POSITION_PACKET 3

// The request packet donent even really need to even call PackData, everything can sort of be done in the constructor, but just in case you want to route em to another module or something its like this for now

// Connection Request Protocol, Uint8 routingTag, int packetType, std::string requestMessage
class ConnectionRequestPacket : public sf::Packet
{
public:
	ConnectionRequestPacket();
	void PackData(sf::Uint8 inputRoutingTag, sf::Packet &packetInstance);
};

// Connection Response Protocol, Uint8 routingTag, int packetType, std::string responseMessage, int playerID
class ConnectionResponsePacket : public sf::Packet
{
public:
	ConnectionResponsePacket();
	void PackData(sf::Uint8 inputRoutingTag, std::string inputResponseMessage, int playerID, sf::Packet &packetInstance);
};

// Connection Validity Protocol, Uint8 routingTag, int packetType, int playerID, long int stateIteration, bool connectionActive
// Note : At 60fps, the stateIterator will not overflow for over 9000 (not even kidding, it's 9942 hours) hours. That's over a year of running. You can even double that by using unsigned if you like, you know, for them really bitter grudgematch games that take over 2 years.
class ConnectionValidityPacket : public sf::Packet
{
public:
	ConnectionValidityPacket();
	void PackData(sf::Uint8 inputRoutingTag, int playerID, int stateIteration, bool connectionActive, sf::Packet &packetInstance);
};

class PositionPacket : public sf::Packet
{
public:
	PositionPacket();
	void PackData(sf::Uint8 routingTag, sf::Int16 xPos, sf::Int16 yPos, sf::Packet &packetInstance);
};
