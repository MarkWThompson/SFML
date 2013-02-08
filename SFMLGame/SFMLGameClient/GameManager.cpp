#include "GameManager.h"

GameManager::GameManager()
{
	curState = NULL;

	clientTransmitter = new ClientTransmitter();
	clientReceiver = new ClientReceiver();
	clientConnector = new ClientConnector();
	clientRouter = new ClientRouter(*clientConnector);

	SwitchState(SharedConstants::START_SCREEN);
}

GameManager::~GameManager()
{
	delete startScreen;
	startScreen = NULL;
	
	delete clientTransmitter;
	clientTransmitter = NULL;
	
	delete clientReceiver;
	clientReceiver = NULL;
	
	delete clientConnector;
	clientConnector = NULL;
	
	delete clientRouter;
	clientRouter = NULL;
}

void GameManager::Update(sf::Event events, const sf::Input &input)
{
	if(curState != NULL)
	{
		// Switch the state if a signal has been given from the current state
		if(curState->Switch())
		{
			SwitchState(curState->GetTarget());
		}

		curState->Update(events, input);
	}
}

void GameManager::SwitchState(SharedConstants::StateID stateID)
{
	// Delete previous state, if set
	if(curState != NULL)
	{
		delete curState;
		curState = NULL;
	}

	switch(stateID)
	{
		case SharedConstants::START_SCREEN:
			curState = new StartScreen(*clientRouter, *clientTransmitter, *clientReceiver, *clientConnector, "impact.ttf", 50.0f, 680); // The string probably needs to be a constant somewhere not in shared constants though
			break;

		case SharedConstants::GAME_SCREEN:
			// curState = new GameScreen();
			break;
	}

	if(curState->Load() == false)
	{
		OutputDebugString("Failed to load state");
	}
}

void GameManager::Draw(sf::RenderWindow &renderWindow)
{
	if(curState != NULL)
	{
		curState->Draw(renderWindow);
	}
}
