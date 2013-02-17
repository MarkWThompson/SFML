#include "PacketTypes.h"
#include <iostream>


ConnectionRequestPacket::ConnectionRequestPacket()
{
}
void ConnectionRequestPacket::PackData(sf::Uint8 inputRoutingTag, sf::Packet &packetInstance)
{
	//Pack a packet, that's fun to say. PackaPacketPackaPacketPackaPacket Hey!
	std::string requestMessage = sharedConstants.GetRequestMessage();
	packetInstance << inputRoutingTag << static_cast<sf::Uint8>(CONNECTION_REQUEST_PACKET) << requestMessage;
}


ConnectionResponsePacket::ConnectionResponsePacket()
{
}
void ConnectionResponsePacket::PackData(sf::Uint8 inputRoutingTag, std::string inputResponseMessage, int playerID, sf::Packet &packetInstance)
{
	//Pack a packet, that's fun to say. PackaPacketPackaPacketPackaPacket Hey!
	std::string responseMessage = inputResponseMessage;
	packetInstance << inputRoutingTag << static_cast<sf::Uint8>(CONNECTION_RESPONSE_PACKET) << responseMessage << playerID;
}


ConnectionValidityPacket::ConnectionValidityPacket()
{
}
void ConnectionValidityPacket::PackData(sf::Uint8 inputRoutingTag, int playerID, int stateIteration, bool connectionActive, sf::Packet &packetInstance)
{
	//Pack a packet, that's fun to say. PackaPacketPackaPacketPackaPacket Hey!
	
	packetInstance << inputRoutingTag << static_cast<sf::Uint8>(CONNECTION_VALIDITY_PACKET) << playerID << stateIteration << connectionActive;
}


PositionPacket::PositionPacket()
{
}
void PositionPacket::PackData(sf::Uint8 routingTag, sf::Int16 xPos, sf::Int16 yPos, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(POSITION_PACKET) << xPos << yPos;
}