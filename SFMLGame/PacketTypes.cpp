#include "PacketTypes.h"
#include <iostream>

// Pack a packet, that's fun to say. PackaPacketPackaPacketPackaPacket Hey!

ConnectionRequestPacket::ConnectionRequestPacket()
{
}
void ConnectionRequestPacket::PackData(sf::Uint8 inputRoutingTag, sf::Packet &packetInstance)
{
	
	std::string requestMessage = sharedConstants.GetRequestMessage();
	packetInstance << inputRoutingTag << static_cast<sf::Uint8>(CONNECTION_REQUEST_PACKET) << requestMessage;
}


ConnectionResponsePacket::ConnectionResponsePacket()
{
}
void ConnectionResponsePacket::PackData(sf::Uint8 routingTag, std::string responseMessage, int playerID, sf::Uint32 stateIteration, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(CONNECTION_RESPONSE_PACKET) << responseMessage << playerID << stateIteration;
}


ConnectionValidityPacket::ConnectionValidityPacket()
{
}
void ConnectionValidityPacket::PackData(sf::Uint8 routingTag, int playerID, sf::Uint32 stateIteration, bool connectionActive, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(CONNECTION_VALIDITY_PACKET) << playerID << stateIteration << connectionActive;
}


PositionPacket::PositionPacket()
{
}
void PositionPacket::PackData(sf::Uint8 routingTag, sf::Int16 xPos, sf::Int16 yPos, sf::Uint32 stateIteration, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(POSITION_PACKET) << xPos << yPos << stateIteration;
}


PlayerInputPacket::PlayerInputPacket()
{
}
void PlayerInputPacket::PackData(sf::Uint8 routingTag, bool wDown, bool aDown, bool sDown, bool dDown, bool spaceDown, bool escDown, 
								 bool returnDown, bool lBtnDown, bool rBtnDown, sf::Int16 mouseX, sf::Int16 mouseY, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(PLAYER_INPUT_PACKET) << wDown << aDown << sDown << dDown << spaceDown << escDown << returnDown << lBtnDown << rBtnDown << mouseX << mouseY;
}
