#include "GameManager.h"
#include "../SharedConstants.h"

/* I'll make this class into an FSM, but for now I'll leave as is. */

GameManager::GameManager()
{
}

GameManager::GameManager(sf::RenderWindow &renderWindow)
{
	//Set state machine bools
	isOnStartScreen = true;
	isOnGame = false;

	//mainly used because keyboards are cunts and send messages too quickly
	canConnect = true;

	clientTransmitter = new ClientTransmitter();
	clientReceiver = new ClientReceiver();
	clientConnector = new ClientConnector();
	clientRouter = new ClientRouter(*clientConnector);
	listenForApproval = false;

	startScreen = new StartScreen(renderWindow, *clientRouter, *clientTransmitter, *clientReceiver, *clientConnector, "impact.ttf", 50.0f, 680); // The string probably needs to be a constant somewhere not in shared constants though

}

GameManager::~GameManager()
{
	startScreen = NULL;
	delete startScreen;

	clientTransmitter = NULL;
	delete clientTransmitter;

	clientReceiver = NULL;
	delete clientReceiver;

	clientConnector = NULL;
	delete clientConnector;

	clientRouter = NULL;
	delete clientRouter;
}

void GameManager::Update(sf::Event events, const sf::Input &input)
{
	if(isOnStartScreen == true)
	{
		// This should probably be put somewhere else - requires the FSM
		startScreen->Update(events, input);
	}
	else if(isOnGame == true)
	{
		
	}

	//Update the Temporary states til the super duper FSM is implement(still think these else ifs are fine, theres only gonna be like 3-4 states, tops)
	if(startScreen->IsConnected() == false)
	{
		isOnStartScreen = true;
		isOnGame = false;
	}
	else if(startScreen->IsConnected() == true)
	{
		isOnStartScreen = false;
		isOnGame = true;
	}

	
}

void GameManager::Draw(sf::RenderWindow &renderWindow)
{
	if(isOnStartScreen == true)
	{
		startScreen->Draw(renderWindow);
	}
	else if(isOnGame == true)
	{
		
	}
}
