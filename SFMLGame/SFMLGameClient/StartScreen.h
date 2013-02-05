#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientTransmitter.h"
#include "ClientReceiver.h"
#include "ClientConnector.h"
#include "ClientRouter.h"
#include "TextInputBox.h"
#include <iostream>
#include "..\PacketTypes.h"

class StartScreen
{
public:
	StartScreen();
	StartScreen(sf::RenderWindow &renderWindow, ClientRouter &inputClientRouter, ClientTransmitter &inputClientTransmitter, ClientReceiver &inputClientReceiver, ClientConnector &inputClientConnector, std::string ipFontPath, float fontSize, int boxWidth);
	~StartScreen();

	void Init();
	void Update(sf::Event events,const sf::Input &input);
	void Draw(sf::RenderWindow &renderWindow);
	sf::IPAddress GetAndEraseIP();

	bool IsConnected();

private:
	ClientTransmitter *clientTransmitter;
	ClientReceiver *clientReceiver;
	ClientConnector *clientConnector;
	ClientRouter *clientRouter;

	sf::Image startScreenImage;
	sf::Sprite startScreen;
	TextInputBox* ipInput;
	bool isConnecting;
	bool canConnect;
	bool listenForApproval;
	bool isConnected;

};

