#include "StartState.h"

StartState::StartState(ClientTransmitter* clientTransmitter, std::string ipFontPath, float fontSize, int boxWidth, ServerNetworkData* serverNetworkData)
	: State(SharedConstants::START_STATE), CONNECTION_ATTMEPTS(4), CONNETION_TIME_INTERVAL(1.0f)
{
	this->clientTransmitter = clientTransmitter;
	this->serverNetworkData = serverNetworkData;

	// Create new text box
	ipInput = new TextInputBox(380.0f, 311.0f, boxWidth, fontSize, ipFontPath);
	playerNameInput = new TextInputBox(485.0f, 575.0f,270,fontSize/2, ipFontPath);

	// Set unset flags
	isConnecting = false;
	canConnect = true;
	listenForApproval = false;
	canReceive = true;

	//Text box stuff
	ipBoxActive = true;
	nameBoxActive = false;

	// Connection request iterator set to above the connection attempts so it doesn't just keep trying to connect immediately
	connectionRequestIterator = CONNECTION_ATTMEPTS + 1;

	
}

void StartState::SetView(sf::RenderWindow* renderWindow)
{
	// Use the default view for the renderWindow
	sf::View& defaultView = renderWindow->GetDefaultView();
	renderWindow->SetView(defaultView);
}


bool StartState::Load()
{
	// Load in the background image
	if(startScreenImage.LoadFromFile(START_STATE_IMAGE))
	{
		startScreen.SetImage(startScreenImage);
		startScreen.SetPosition(0.0f, 0.0f);
	}
	else
	{
		return false;
	}

	// Load in the connection image
	if(connectionImage.LoadFromFile(CONNECTING_IMAGE))
	{
		connectionIcon.SetImage(connectionImage);
		connectionIcon.SetPosition(1200.0f, 30.0f);
	}
	else
	{
		return false;
	}

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
	HandleActiveInputBoxes(events, eventFired, input);

	if(ipBoxActive == true)
	{
		ipInput->Update(events, eventFired, input);
	}
	if(nameBoxActive == true)
	{
		playerNameInput->Update(events, eventFired, input);
	}

	if((input.IsKeyDown(sf::Key::Return)) && (canConnect == true))
	{
		candidateServer = GetAndEraseInput(ipInput);
		inputtedName = GetAndEraseInputString(playerNameInput);
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
		sf::Vector2f playerDimensions;
		playerDimensions.x = (float)playerImage.GetWidth();
		playerDimensions.y = (float)playerImage.GetHeight();
		packetToSend.PackData(sharedConstants.CONNECT_MODULE, inputtedName, packetToSend);
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

	if(packetType == CONNECTION_RESPONSE_PACKET) // Connection responses
	{
		std::string requestResponse;
		receivedPacket >> requestResponse;

		if(requestResponse == sharedConstants.GetAcceptMessage())
		{
			std::cout << std::endl << "Connection approved by server : " + connectionAddress.ToString() << std::endl;

			// Unpack this players number
			receivedPacket >> playerID;

			// Unpack max player info
			sf::Uint8 receivedMaxPlayers;
			receivedPacket >> receivedMaxPlayers;

			// Unpack level 
			std::string levelFilepath;
			receivedPacket >> levelFilepath;

			// Set server network data
			serverNetworkData->maxPlayers = receivedMaxPlayers;
			serverNetworkData->playerID = playerID;
			serverNetworkData->serverIP = connectionAddress;
			serverNetworkData->serverPort = port;
			serverNetworkData->serverLevel = levelFilepath;
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

void StartState::Draw(sf::RenderWindow* renderWindow)
{
	renderWindow->Draw(startScreen);
	
	// Render text input boxes
	ipInput->Draw(renderWindow);
	playerNameInput->Draw(renderWindow);

	// Display connecting icon when listening for server response
	if(listenForApproval == true)
	{
		renderWindow->Draw(connectionIcon);
	}
}

sf::IPAddress StartState::GetAndEraseInput(TextInputBox *inputBox)
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

std::string StartState::GetAndEraseInputString(TextInputBox *inputBox)
{
	std::string returnString;
	std::string input = inputBox->sendBuffer;
	if(input != "")
	{
		returnString = input;
		inputBox->EraseBuffers();
	}
	else
	{
		returnString.clear();
	}
	
	return returnString;
}


void StartState::HandleActiveInputBoxes(sf::Event events, bool eventFired, const sf::Input &input)
{

	//int mouseY = events.MouseMove.Y;
	if((events.Type == sf::Event::MouseButtonPressed) && (eventFired == true))
	{
		if(ipInput->bounds.Contains((float)input.GetMouseX(), (float)input.GetMouseY()))
		{
			ipBoxActive = true;
			nameBoxActive = false;
		}

		if(playerNameInput->bounds.Contains((float)input.GetMouseX(), (float)input.GetMouseY()))
		{
			ipBoxActive = false;
			nameBoxActive = true;
		}
	}
}
