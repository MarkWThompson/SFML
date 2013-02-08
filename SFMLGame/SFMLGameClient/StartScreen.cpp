#include "StartScreen.h"

StartScreen::StartScreen()
{
	ipInput = new TextInputBox(260, 240, 700, 50, "Impact.ttf");
}

StartScreen::StartScreen(ClientRouter &clientRouter, ClientTransmitter &clientTransmitter, ClientReceiver &clientReceiver, ClientConnector &clientConnector, std::string ipFontPath, float fontSize, int boxWidth)
{
	ipInput = new TextInputBox(260.0f, 240.0f, boxWidth, fontSize, ipFontPath);
	this->clientTransmitter = &clientTransmitter;
	this->clientReceiver = &clientReceiver;
	this->clientConnector = &clientConnector;
	this->clientRouter = &clientRouter;
}

bool StartScreen::Load()
{
	// Load in the image
	if(startScreenImage.LoadFromFile("StartScreen.png"))
	{
		startScreen.SetImage(startScreenImage);
	}
	else
	{
		return false;
	}

	startScreen.SetPosition(0.0f, 0.0f);
	isConnecting = false;

	// Mainly used because keyboards are cunts and send messages too quickly
	canConnect = true;
	listenForApproval = false;

	return true;
}

StartScreen::~StartScreen()
{
	delete clientTransmitter;
	delete clientReceiver;
	delete clientConnector;
	delete clientRouter;

	clientTransmitter = NULL;
	clientReceiver = NULL;
	clientConnector = NULL;
	clientRouter = NULL;
}

bool StartScreen::IsConnected()
{
	return isConnected;
}

void StartScreen::Update(sf::Event events, const sf::Input &input)
{
	ipInput->Update(events);
	if((input.IsKeyDown(sf::Key::Return)) && (canConnect == true))
	{
		canConnect = false;

		// Send connection request
		ConnectionRequestPacket packetToSend;
		packetToSend.PackData(0, packetToSend);
		clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), GetAndEraseIP(), packetToSend);
		listenForApproval = true;
	}
	if(listenForApproval == true)
	{
		clientReceiver->ReceiveUDP(sharedConstants.GetClientReceivePort(),*clientRouter,true);
		listenForApproval = false;
		if(clientConnector->isConnected == true)
		{
			isConnected = true;
		}

	}
	if(input.IsKeyDown(sf::Key::Return) == false)
	{
		// Dont delete or we connect too much when we hit enter
		canConnect = true;
	}
}

void StartScreen::Draw(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(startScreen);
	ipInput->Draw(renderWindow);
}

sf::IPAddress StartScreen::GetAndEraseIP()
{
	sf::IPAddress returnAddress = ipInput->sendBuffer;
	ipInput->EraseBuffers();

	return returnAddress;
}
