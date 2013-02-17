#include "GameManager.h"

GameManager::GameManager()
{
	curState = NULL;

	// PIPES
	clientTransmitter = new ClientTransmitter();
	clientReceiver = new ClientReceiver(false);
	
	SwitchState(SharedConstants::START_STATE);
}

GameManager::~GameManager()
{
	// States
	delete startState;
	delete gameState;
	startState = NULL;
	gameState = NULL;

	// PIPES
	delete clientTransmitter;
	delete clientReceiver;
	clientTransmitter = NULL;
	clientReceiver = NULL;
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

	// Will receive data if the state allows it
	if(curState->CanReceive())
	{
		clientReceiver->ReceiveUDP(curState);
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

	std::cout << "State switched to: ";

	switch(stateID)
	{
		case SharedConstants::START_STATE:
			std::cout << "START_STATE." << std::endl;
			curState = new StartState(clientTransmitter, "impact.ttf", 50.0f, 680); // The string probably needs to be a constant somewhere not in shared constants though
			break;

		case SharedConstants::GAME_STATE:
			std::cout << "GAME_STATE." << std::endl;
			curState = new GameState(clientTransmitter);
			break;
	}

	if(curState->Load() == false)
	{
		std::cout << "Failed to load state with ID : " << stateID << std::endl;
	}
}

void GameManager::Draw(sf::RenderWindow &renderWindow)
{
	if(curState != NULL)
	{
		curState->Draw(renderWindow);
	}
}
