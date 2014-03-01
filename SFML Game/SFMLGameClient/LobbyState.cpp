#include "LobbyState.h"
#include "NotificationBox.h"
#include "Stringify.h"
#include "AudioManager.h"
#include "SimplePersistence.h"

LobbyState::LobbyState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData)
	: MenuState(SharedConstants::LOBBY_STATE), CONNECTION_ATTMEPTS(4), 
	CONNETION_TIME_INTERVAL(1.0f), CONNECTION_LOAD_TIME(1.0f),
	connectButton(600, 40, 120, 40)
{
	this->clientTransmitter = clientTransmitter;
	this->serverNetworkData = serverNetworkData;

	// Create new input box
	ipInputBox = new InputBox(112.0f, 248.0f, 470, 60, 470, 10, 30.0f, MOIRE_FONT_FILE);
	
	isConnected = false;
	isConnecting = false;
	canConnect = true;
	initialiseConnection = false;
	listenForApproval = false;
	ipInputBoxActive = true;
	candidateServerIP = NULL_IP;
	notificationID = -1;

	// "CONNECTION_ATTMEPTS + 1" to prevent immediate connection attempts
	connectionRequestIterator = CONNECTION_ATTMEPTS + 1;

	if(audioManager.menuMusic.GetStatus() != sf::Sound::Playing)
	{
		audioManager.menuMusic.Play();
	}
	audioManager.gameMusic.Stop();
}

bool LobbyState::MenuLoad()
{
	if(connectButtonImage.LoadFromFile(CONNECT_BUTTON_IMAGE))
	{
		connectButton.SetImage(connectButtonImage);
		connectButton.SetPosition(110.0f, 318.0f);
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

	if(overlayImage.LoadFromFile(LOBBY_OVERLAY_IMAGE))
	{
		overlaySprite.SetImage(overlayImage);
	}
	else
	{
		return false;
	}

	StartListening();

	return true;
}

LobbyState::~LobbyState()
{
	clientTransmitter = NULL;
	serverNetworkData = NULL;
}

void LobbyState::MenuUpdate(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
	if(ipInputBoxActive == true)
	{
		ipInputBox->Update(events, eventFired, INPUT);
	}

	if(canConnect)
	{
		if(INPUT.IsKeyDown(sf::Key::Return) || connectButton.Pressed(sf::Mouse::Left, INPUT))
		{
			candidateServerIP = ParseInputBox(ipInputBox);
		
			if(candidateServerIP != NULL_IP)
			{
				connectionRequestIterator = 0;
				canConnect = false;
				connectButton.Disable();
				listenForApproval = true;
				ipInputBoxActive = false;
				notificationID = notificationBox.AddNotification("Requesting connection to server.", false);
			}
		}
	}

	if(listenForApproval)
	{
		// If we're ready to send another request packet
		if((connectionAttemptIntervalClock.GetElapsedTime() > CONNETION_TIME_INTERVAL) && (connectionRequestIterator < CONNECTION_ATTMEPTS))
		{
			ConnectionRequestPacket packetToSend;
			sf::Vector2f playerDimensions;
			playerDimensions.x = (float)playerImage.GetWidth();
			playerDimensions.y = (float)playerImage.GetHeight();
			packetToSend.PackData(sharedConstants.CONNECT_MODULE, sPersistence.GetString("Username"), packetToSend);
			clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), candidateServerIP, packetToSend);
			connectionRequestIterator++;
			connectionAttemptIntervalClock.Reset();

			if(connectionRequestIterator >= CONNECTION_ATTMEPTS)
			{
				std::string message = "Failed to establish connection to server after " + Stringify(CONNECTION_ATTMEPTS) + " tries.";
				notificationBox.RemoveNotification(notificationID);
				notificationBox.AddNotification(message, true);

				listenForApproval = false;
				connectButton.Enable();
				canConnect = true;
				ipInputBoxActive = true;
				ipInputBox->EraseBuffers();
			}
			else
			{
				// Display progress
				notificationBox.RemoveNotification(notificationID);
				std::string message = "Requesting connection, trying: " + Stringify(connectionRequestIterator) + ".";
				notificationID = notificationBox.AddNotification(message, false);
			}
		}
	}

	if(initialiseConnection == true)
	{
		std::cout << "Connection accepted. Now loading." << std::endl;

		isConnecting = true;
		connectionClock.Reset();
		notificationBox.RemoveNotification(notificationID);
		notificationID = notificationBox.AddNotification("Connection accepted. Now loading.", false);

		initialiseConnection = false;
	}

	if(isConnecting)
	{
		if(connectionClock.GetElapsedTime() > CONNECTION_LOAD_TIME)
		{
			notificationBox.RemoveNotification(notificationID);
			isConnected = true;
			isConnecting = false;
		}
	}

	if(isConnected)
	{
		SwitchState(SharedConstants::GAME_STATE);
	}

	connectButton.Update(sf::Mouse::Left, INPUT);
}

void LobbyState::MenuDraw(sf::RenderWindow* renderWindow)
{
	// Display connecting icon when listening for server response
	if(listenForApproval == true)
	{
		renderWindow->Draw(connectionIcon);
	}

	// Render text input boxes
	ipInputBox->Draw(renderWindow);

	renderWindow->Draw(connectButton);
}

void LobbyState::SetView(sf::RenderWindow* renderWindow)
{
	// Use the default view for the renderWindow
	sf::View& defaultView = renderWindow->GetDefaultView();
	renderWindow->SetView(defaultView);
	renderWindow->ShowMouseCursor(true);
}

void LobbyState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
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
			initialiseConnection = true;
			listenForApproval = false;

			std::cout << "Player ID : " << playerID << std::endl;
		}
		else if(requestResponse == sharedConstants.GetRejectMessage())
		{
			std::cout << std::endl << "Connection denied by server : " + connectionAddress.ToString() << std::endl;
			initialiseConnection = false;
		}
	}
}

sf::IPAddress LobbyState::ParseInputBox(InputBox *inputBox)
{
	sf::IPAddress returnAddress;

	std::string input = ipInputBox->GetInput();

	if(input != "")
	{
		returnAddress = input;
	}
	else
	{
		returnAddress = NULL_IP;
	}
	
	return returnAddress;
}
