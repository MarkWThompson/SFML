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
#include "State.h"

class StartState : public State
{
public:
	StartState();
	StartState(ClientRouter &clientRouter, ClientTransmitter &clientTransmitter, ClientReceiver &clientReceiver, ClientConnector &clientConnector, std::string ipFontPath, float fontSize, int boxWidth);
	~StartState();

	bool Load();
	void Update(sf::Event events, const sf::Input &input);
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

