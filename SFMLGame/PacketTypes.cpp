#include "PacketTypes.h"
#include <iostream>

// Pack a packet, that's fun to say. PackaPacketPackaPacketPackaPacket Hey!


//Now we can pack vectors and shiz
sf::Packet& operator <<(sf::Packet& Packet, const sf::Vector2f& inputVector2f)
{
    return Packet << inputVector2f.x << inputVector2f.y;
}

sf::Packet& operator >>(sf::Packet& Packet, sf::Vector2f& outputVector2f)
{
    return Packet >> outputVector2f.x >> outputVector2f.y;
}


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
void ConnectionResponsePacket::PackData(sf::Uint8 routingTag, std::string responseMessage, int playerID, sf::Uint8 maxPlayers, sf::Uint32 stateIteration, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(CONNECTION_RESPONSE_PACKET) << responseMessage << playerID << maxPlayers << stateIteration;
}

DisconnectionPacket::DisconnectionPacket()
{
}
void DisconnectionPacket::PackData(sf::Uint8 routingTag, int playerID, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(DISCONNECTION_PACKET) << playerID;
}

ConnectionValidityPacket::ConnectionValidityPacket()
{
}
void ConnectionValidityPacket::PackData(sf::Uint8 routingTag, int playerID, sf::Uint32 stateIteration, bool connectionActive, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(CONNECTION_VALIDITY_PACKET) << playerID << stateIteration << connectionActive;
}


PlayerPositionsPacket::PlayerPositionsPacket()
{
}

//playerNum MUST be the size of the array, the array size should really be the amount of players there are.
void PlayerPositionsPacket::PackData(sf::Uint8 routingTag, sf::Uint8 playerPosVectorSize, std::vector<bool> playersActive, std::vector<sf::Vector2f> playerPositions,sf::Uint32 stateIteration, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(PLAYER_POSITIONS_PACKET);
	//It really is important that playerPosVector size is the same size as playersActive and playerPositions, this will tell you if its not
	if((playerPosVectorSize != playersActive.size()) || (playerPosVectorSize != playerPositions.size()))
	{
		std::cout << "playerPosVectorSize in PlayerPositionsPacket isn't the correct size!! THIS WILL FUCK SHIZ UP" << std::endl;
	}
	packetInstance << playerPosVectorSize;

	for(int i = 0; i < playersActive.size(); i++)
	{
		packetInstance << playersActive[i];
	}

	for(int i = 0; i < playerPositions.size(); i++)
	{
		packetInstance << playerPositions[i];
	}
	packetInstance << stateIteration;
}



PlayerInputPacket::PlayerInputPacket()
{
}
void PlayerInputPacket::PackData(sf::Uint8 routingTag, int playerID, bool playerActive, bool wDown, bool aDown, bool sDown, bool dDown, bool spaceDown, bool escDown, 
								 bool returnDown, bool lBtnDown, bool rBtnDown, sf::Int16 mouseX, sf::Int16 mouseY, sf::Packet &packetInstance)
{
	packetInstance << routingTag << static_cast<sf::Uint8>(PLAYER_INPUT_PACKET) << playerID << wDown << aDown << sDown << dDown << spaceDown << escDown << returnDown << lBtnDown << rBtnDown << mouseX << mouseY;
}
