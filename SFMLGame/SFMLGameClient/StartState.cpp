#include "StartState.h"

StartState::StartState(ClientTransmitter* clientTransmitter, std::string ipFontPath, float fontSize, int boxWidth)
	: State(SharedConstants::START_STATE)
{
	this->clientTransmitter = clientTransmitter;

	// Create new text box
	ipInput = new TextInputBox(260.0f, 240.0f, boxWidth, fontSize, ipFontPath);

	// Set unset flags
	isConnecting = false;
	canConnect = true;
	listenForApproval = false;
	canReceive = true;
}

bool StartState::Load()
{
	// Load in the image
	if(startScreenImage.LoadFromFile("StartState.png"))
	{
		startScreen.SetImage(startScreenImage);
	}
	else
	{
		return false;
	}

	startScreen.SetPosition(0.0f, 0.0f);

	return true;
}

StartState::~StartState()
{
	clientTransmitter = NULL;
}

bool StartState::IsConnected()
{
	return isConnected;
}

void StartState::Update(sf::Event events, const sf::Input &input)
{
	ipInput->Update(events);

	if((input.IsKeyDown(sf::Key::Return)) && (canConnect == true))
	{
		canConnect = false;

		// Send connection request
		ConnectionRequestPacket packetToSend;
		packetToSend.PackData(sharedConstants.CONNECT_MODULE, packetToSend);
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), GetAndEraseIP(), packetToSend);
		listenForApproval = true;
	}

	if(listenForApproval == true)
	{
		if(isConnected == true)
		{
			targetID = SharedConstants::GAME_STATE;
		}
	}

	if(input.IsKeyDown(sf::Key::Return) == false)
	{
		// Dont delete or we connect too much when we hit enter
		canConnect = true;
	}
}

void StartState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
	// A connectionResponse packet , Uint8 formatTag, std::string responseString, Uint8 playerNumber
	// PACKET MUST HAVE A FORMATTAG AS THE FIRST ENTRY, TO FACILITATE MULTIPLE PACKET TYPES
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

			serverIP = connectionAddress;
			this->serverPort = port;
			isConnected = true;

			std::cout << "Player ID : " << playerID;
		}
		else if(requestResponse == sharedConstants.GetRejectMessage())
		{
			std::cout << std::endl << "Connection denied by server : " + connectionAddress.ToString() << std::endl;

			serverIP = connectionAddress;
			this->serverPort = port;
			isConnected = false;
		}
	}
}

void StartState::Draw(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(startScreen);
	ipInput->Draw(renderWindow);
}

sf::IPAddress StartState::GetAndEraseIP()
{
	sf::IPAddress returnAddress = ipInput->sendBuffer;
	ipInput->EraseBuffers();

	return returnAddress;
}
