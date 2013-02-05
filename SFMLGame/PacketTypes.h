#pragma once
#include "c:\sfml-1.6\include\sfml\network\packet.hpp"
#include "SharedConstants.h"


#define CONNECTION_REQUEST_PACKET 0
#define CONNECTION_RESPONSE_PACKET 1

//the request packet donent even really need to even call PackData, everything can sort of be done in the constructor, but just in case you want to route em to another module or something its like this for now

//Connection Request Protocol, Uint8 routingTag, int packetType, std::string requestMessage
class ConnectionRequestPacket : public sf::Packet
{
public:
	ConnectionRequestPacket(void);
	void PackData(sf::Uint8 inputRoutingTag, sf::Packet &packetInstance);
};

//Connection Response Protocol, Uint8 routingTag, int packetType, std::string responseMessage
class ConnectionResponsePacket : public sf::Packet
{
public:
	ConnectionResponsePacket(void);
	void PackData(sf::Uint8 inputRoutingTag, std::string inputResponseMessage, sf::Packet &packetInstance);
};

