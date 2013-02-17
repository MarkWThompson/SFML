#include "GameState.h"

GameState::GameState(ClientTransmitter* clientTransmitter) 
	: State(SharedConstants::GAME_STATE)
{
	this->clientTransmitter = clientTransmitter;
}

GameState::~GameState()
{
	clientTransmitter = NULL;
}

bool GameState::Load()
{
	// Load in the image
	if(androidImage.LoadFromFile("Android.png"))
	{
		androidSprite.SetImage(androidImage);
	}
	else
	{
		return false;
	}

	androidSprite.SetPosition(0.0f, 0.0f);

	// Will now listen for and receive packets
	canReceive = true;

	return true;
}

void GameState::Update(sf::Event events, const sf::Input &input)
{

}

void GameState::Draw(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(androidSprite);
}

void GameState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	// Grab packet type
	sf::Uint8 packetType;
	receivedPacket >> packetType;

	// Temp stack variables
	sf::Int16 xPos, yPos;

	if(packetType == POSITION_PACKET)
	{
		std::cout << "Packet type: POSITION_PACKET" << std::endl;
		// Grab coords
		receivedPacket >> xPos >> yPos;
		androidSprite.SetPosition(xPos,yPos);
	}
	else
	{
		std::cout << "Packet type: UNDEFINED_PACKET" << std::endl;
	}
}
