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
	if(playerImage.LoadFromFile(MEGAMAN_IMAGE))
	{
		for(int i = 0; i < serverNetworkData->maxPlayers; i++)
		{
			sf::Sprite setupSprite;
			playerSprites.push_back(setupSprite);	//Create an empty pointer vector of 8 sprites
			playerSprites[i].SetImage(playerImage);
			// Set no players to be active at the start
			playersActive.push_back(false);
		}
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


		//Reset these all to false because they sometimes get stuck due to window management and shiz

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
		inputPacket.PackData(sharedConstants.GAME_MODULE,serverNetworkData->playerID, true, wDown, aDown, sDown, dDown, spaceDown, escDown, returnDown, lBtnDown, rBtnDown, mouseX, mouseY, inputPacket);

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


	if(packetType == PLAYER_POSITIONS_PACKET)
	{
		UnpackPlayerPositionsPacket(receivedPacket, playersActive, playerSprites, stateIterator);
	}
	else
	{
		std::cout << "Packet type: UNDEFINED_PACKET" << std::endl;
	}
}

void GameState::UnpackPlayerPositionsPacket(sf::Packet &receivedPacket, std::vector<bool> &playersActive, std::vector<sf::Sprite> &playerSprites, sf::Uint32 &stateIterator)
{
	sf::Uint8 playerPositionVectorSize;
	receivedPacket >> playerPositionVectorSize;

	//Grab what players are active
	for(int i = 0; i < playerPositionVectorSize; i++)
	{
		bool isThisOneActive;
		receivedPacket >> isThisOneActive;
		playersActive[i] = isThisOneActive;
	}

	// Grab coords
	for(int i = 0; i < playerPositionVectorSize; i++)
	{
		sf::Vector2f playerPos;
		receivedPacket >> playerPos;
		playerSprites[i].SetPosition(playerPos);
	}
		
	// Set state iteration variable
	sf::Uint32 stateIteration;
	receivedPacket >> stateIteration;
	stateIterator = stateIteration;
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
	//render all the players
	for(int i = 0; i < serverNetworkData->maxPlayers; i++)
	{
		if(playersActive[i])
		{
			renderWindow.Draw(playerSprites[i]);
		}
	}
	//renderWindow.Draw(androidSprite);

	if(connectionProblem == true)
	{
		renderWindow.Draw(interuptSprite);
	}
}
