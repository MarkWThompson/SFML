#include "PacketTypes.h"
#include <iostream>


ConnectionRequestPacket::ConnectionRequestPacket(void)
{
}
void ConnectionRequestPacket::PackData(sf::Uint8 inputRoutingTag, sf::Packet &packetInstance)
{
	//Pack a packet, that's fun to say. PackaPacketPackaPacketPackaPacket Hey!
	std::string requestMessage = sharedConstants.GetRequestMessage();
	packetInstance << inputRoutingTag << static_cast<sf::Uint8>(CONNECTION_REQUEST_PACKET) << requestMessage;
}


ConnectionResponsePacket::ConnectionResponsePacket(void)
{
}
void ConnectionResponsePacket::PackData(sf::Uint8 inputRoutingTag, std::string inputResponseMessage, sf::Packet &packetInstance)
{
	//Pack a packet, that's fun to say. PackaPacketPackaPacketPackaPacket Hey!
	std::string responseMessage = inputResponseMessage;
	packetInstance << inputRoutingTag << static_cast<sf::Uint8>(CONNECTION_REQUEST_PACKET) << responseMessage;
}


