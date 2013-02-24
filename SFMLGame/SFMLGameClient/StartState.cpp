#include "StartState.h"
#include "Assets.h"

StartState::StartState(ClientTransmitter* clientTransmitter, std::string ipFontPath, float fontSize, int boxWidth, ServerNetworkData* serverNetworkData)
	: State(SharedConstants::START_STATE), CONNECTION_ATTMEPTS(4), CONNETION_TIME_INTERVAL(1.0f)
{
	this->clientTransmitter = clientTransmitter;
	this->serverNetworkData = serverNetworkData;

	// Create new text box
	ipInput = new TextInputBox(380.0f, 311.0f, boxWidth, fontSize, ipFontPath);

	// Set unset flags
	isConnecting = false;
	canConnect = true;
	listenForApproval = false;
	canReceive = true;

	// Connection request iterator set to above the connection attempts so it doesn't just keep trying to connect immediately
	connectionRequestIterator = CONNECTION_ATTMEPTS + 1;
}

bool StartState::Load()
{
	// Load in the background image
	if(startScreenImage.LoadFromFile(START_STATE_IMAGE))
	{
		startScreen.SetImage(startScreenImage);
	}
	else
	{
		return false;
	}

	// Load in the connection image
	if(connectionImage.LoadFromFile(CONNECTING_IMAGE))
	{
		connectionIcon.SetImage(connectionImage);
	}
	else
	{
		return false;
	}

	startScreen.SetPosition(0.0f, 0.0f);
	connectionIcon.SetPosition(1200.0f, 30.0f);

	return true;
}

StartState::~StartState()
{
	clientTransmitter = NULL;
	serverNetworkData = NULL;
}

bool StartState::IsConnected()
{
	return isConnected;
}

void StartState::Update(sf::Event events, bool eventFired, const sf::Input &input)
{
	ipInput->Update(events, eventFired, input);

	if((input.IsKeyDown(sf::Key::Return)) && (canConnect == true))
	{
		candidateServer = GetAndEraseInput();
		if(candidateServer != NULL_IP)
		{
			connectionRequestIterator = 0;
			canConnect = false;
			listenForApproval = true;
		}
	}

	// If we're ready to send another request packet
	if((connectionAttemptIntervalClock.GetElapsedTime() > CONNETION_TIME_INTERVAL) && (connectionRequestIterator < CONNECTION_ATTMEPTS))
	{
		ConnectionRequestPacket packetToSend;
		packetToSend.PackData(sharedConstants.CONNECT_MODULE, packetToSend);
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), candidateServer, packetToSend);
		std::cout << "Requesting connection. Trying : " << connectionRequestIterator + 1 << std::endl;
		connectionRequestIterator++;
		connectionAttemptIntervalClock.Reset();

		if(connectionRequestIterator >= CONNECTION_ATTMEPTS)
		{
			std::cout << "Failed to establish connection to server after " << CONNECTION_ATTMEPTS << " tries." << std::endl;
			listenForApproval = false;
		}
	}

	if(isConnected == true)
	{
		std::cout << "Connection established with server." << std::endl;
		targetID = SharedConstants::GAME_STATE;
	}

	if(input.IsKeyDown(sf::Key::Return) == false)
	{
		// Dont delete or we connect too much when we hit enter
		canConnect = true;
	}
}

void StartState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	// Has received data, reset timeout clock
	serverNetworkData->timeoutClock.Reset();

	// A connectionResponse packet: Uint8 formatTag, std::string responseString, Uint8 playerNumber
	sf::Uint8 packetType;
	receivedPacket >> packetType;

	// The format tag as a magic number will probably need to be discussed
	if(packetType == CONNECTION_RESPONSE_PACKET) // Connection responses
	{
		std::string requestResponse;
		receivedPacket >> requestResponse;

		if(requestResponse == sharedConstants.GetAcceptMessage())
		{
			std::cout << std::endl << "Connection approved by server : " + connectionAddress.ToString() << std::endl;

			// Unpack this players number
			receivedPacket >> playerID;

			sf::Uint8 receivedMaxPlayers;
			receivedPacket >> receivedMaxPlayers;

			// Set server network data
			serverNetworkData->maxPlayers = receivedMaxPlayers;
			serverNetworkData->playerID = playerID;
			serverNetworkData->serverIP = connectionAddress;
			serverNetworkData->serverPort = port;
			serverNetworkData->timeoutClock.Reset();
			isConnected = true;

			std::cout << "Player ID : " << playerID << std::endl;
		}
		else if(requestResponse == sharedConstants.GetRejectMessage())
		{
			std::cout << std::endl << "Connection denied by server : " + connectionAddress.ToString() << std::endl;
			isConnected = false;
		}
	}
}

void StartState::Draw(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(startScreen);
	ipInput->Draw(renderWindow);

	// Display connecting icon when listening for server response
	if(listenForApproval == true)
	{
		renderWindow.Draw(connectionIcon);
	}
}

sf::IPAddress StartState::GetAndEraseInput()
{
	sf::IPAddress returnAddress;
	std::string input = ipInput->sendBuffer;
	if(input != "")
	{
		returnAddress = input;
		ipInput->EraseBuffers();
	}
	else
	{
		returnAddress = NULL_IP;
	}
	
	return returnAddress;
}
