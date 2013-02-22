#include "GameState.h"
#include "Assets.h"

GameState::GameState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData) 
	: State(SharedConstants::GAME_STATE)
{
	this->clientTransmitter = clientTransmitter;
	this->serverNetworkData = serverNetworkData;
	stateIterator = 0;
	connectionProblem = false;
}

GameState::~GameState()
{
	clientTransmitter = NULL;
	serverNetworkData = NULL;
}

bool GameState::Load()
{
	// Load the player controlled android sprite
	if(androidImage.LoadFromFile(ANDROID_IMAGE))
	{
		androidSprite.SetImage(androidImage);
	}
	else
	{
		return false;
	}

	// Load the connection problem icon
	if(interuptImage.LoadFromFile(INTERUPT_IMAGE))
	{
		interuptSprite.SetImage(interuptImage);
	}
	else
	{
		return false;
	}

	// Set the initial positions of the sprites
	androidSprite.SetPosition(0.0f, 0.0f);
	interuptSprite.SetPosition(1200.0f, 30.0f);

	// Will now listen for and receive packets
	canReceive = true;

	return true;
}

void GameState::Update(sf::Event events, bool eventFired, const sf::Input &input)
{
	if(serverNetworkData->serverIP != NULL_IP)
	{
		// Create input packet
		PlayerInputPacket inputPacket;

		// Grab state of keys
		bool wDown = input.IsKeyDown(sf::Key::W);
		bool aDown = input.IsKeyDown(sf::Key::A);
		bool sDown = input.IsKeyDown(sf::Key::S);
		bool dDown = input.IsKeyDown(sf::Key::D);
		bool spaceDown = input.IsKeyDown(sf::Key::Space);
		bool escDown = input.IsKeyDown(sf::Key::Escape);
		bool returnDown = input.IsKeyDown(sf::Key::Return);
		bool lBtnDown = input.IsMouseButtonDown(sf::Mouse::Left);
		bool rBtnDown = input.IsMouseButtonDown(sf::Mouse::Right);
		sf::Int16 mouseX = input.GetMouseX();
		sf::Int16 mouseY = input.GetMouseY();

		// Pack input data
		inputPacket.PackData(sharedConstants.GAME_MODULE, wDown, aDown, sDown, dDown, spaceDown, escDown, returnDown, lBtnDown, rBtnDown, mouseX, mouseY, inputPacket);

		// This packet goes to the connectionHandler on the server
		ConnectionValidityPacket validationPacket;
		validationPacket.PackData(SharedConstants::CONNECT_MODULE, serverNetworkData->playerID, stateIterator, true, validationPacket);

		// Send packets
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, inputPacket);
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, validationPacket);
	}

	// The fixed timestep, the only thing this at the moment, and probably ever is update the stateIterator to what we think it is on the server
	if(timeStepClock.GetElapsedTime() > sharedConstants.GetTimeStep())
	{
		timeStepClock.Reset();
		stateIterator++;
	}

	CheckTimeout();
}

void GameState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	// Has received data, reset timeout clock
	serverNetworkData->timeoutClock.Reset();

	// Grab packet type
	sf::Uint8 packetType;
	receivedPacket >> packetType;

	// Temp stack variables
	sf::Int16 xPos, yPos;

	if(packetType == POSITION_PACKET)
	{
		// Grab coords
		receivedPacket >> xPos >> yPos;
		androidSprite.SetPosition(xPos,yPos);

		// Set state iteration variable
		sf::Uint32 stateIteration;
		receivedPacket >> stateIteration;
		stateIterator = stateIteration;
	}
	else
	{
		std::cout << "Packet type: UNDEFINED_PACKET" << std::endl;
	}
}

void GameState::CheckTimeout()
{
	// If the time since the last packet received is greate than the timeout indication time, flag a connection problem
	if(serverNetworkData->timeoutClock.GetElapsedTime() > sharedConstants.GetTimeoutIndicationTime())
	{
		connectionProblem = true;
	}
	else
	{
		connectionProblem = false;
	}

	if(serverNetworkData->timeoutClock.GetElapsedTime() > sharedConstants.GetTimeout())
	{
		serverNetworkData->Reset();

		std::cout << "Lost connetion to the server, now disconnecting..." << std::endl;

		// Go back to start state
		targetID = sharedConstants.START_STATE;
	}
}

void GameState::Draw(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(androidSprite);

	if(connectionProblem == true)
	{
		renderWindow.Draw(interuptSprite);
	}
}
