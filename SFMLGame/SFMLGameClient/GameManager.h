#pragma once

#include "StartScreen.h"
#include "ClientTransmitter.h"
#include "ClientReceiver.h"
#include "ClientConnector.h"
#include "..\PacketTypes.h"

class GameManager
{
public:
	GameManager();
	GameManager(sf::RenderWindow &renderWindow);
	~GameManager();

	StartScreen* startScreen;

	void Update(sf::Event events, const sf::Input &input);
	void Draw(sf::RenderWindow &renderWindow);

private:
	bool isOnStartScreen;
	bool isOnGame;
	bool listenForApproval;
	bool canConnect;

	ClientTransmitter* clientTransmitter;
	ClientReceiver* clientReceiver;
	ClientRouter* clientRouter;
	ClientConnector* clientConnector;

};
