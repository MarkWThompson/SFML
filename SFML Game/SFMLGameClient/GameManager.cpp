#include "GameManager.h"
#include "NotificationBox.h"

GameManager::GameManager(sf::RenderWindow* renderWindow)
{
	this->renderWindow = renderWindow;
	curState = NULL;

	clientTransmitter = new ClientTransmitter();
	clientReceiver = new ClientReceiver();

	// Network data
	serverNetworkData = new ServerNetworkData();
	
	// Initial state
	SwitchState(SharedConstants::SPLASH_STATE);
}

GameManager::~GameManager()
{
	// PIPES
	delete clientTransmitter;
	delete clientReceiver;
	clientTransmitter = NULL;
	clientReceiver = NULL;
}

void GameManager::Update(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
	if(curState != NULL)
	{
		// Switch the state if a signal has been given from the current state
		if(curState->Switch())
		{
			SwitchState(curState->GetTargetState());
		}

		curState->Update(events, eventFired, INPUT);

		notificationBox.Update(INPUT);
	}

	// Will receive data if the state allows it
	if(curState->IsListening())
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
		case SharedConstants::SPLASH_STATE:
			std::cout << "Splash state." << std::endl;
			curState = new SplashState();
			break;
		
		case SharedConstants::LOBBY_STATE:
			std::cout << "Lobby state." << std::endl;
			curState = new LobbyState(clientTransmitter, serverNetworkData);
			break;
			
		case SharedConstants::OPTIONS_STATE:
			std::cout << "Options state." << std::endl;
			curState = new OptionsState();
			break;
		
		case SharedConstants::HELP_STATE:
			std::cout << "Help state." << std::endl;
			curState = new HelpState();
			break;

		case SharedConstants::CREDITS_STATE:
			std::cout << "Credits state." << std::endl;
			curState = new CreditsState();
			break;
		

		case SharedConstants::GAME_STATE:
			std::cout << "Game state." << std::endl;
			curState = new GameState(clientTransmitter, serverNetworkData);
			break;
	}

	// When a state fails to load, this handles it
	if(curState->Load() == false)
	{
		std::cout << "Failed to load state with ID : " << stateID << std::endl;
		
		// Delete state which failed to load
		delete curState;
		curState = NULL;

		// Boots back to main menu if failed to load
		SwitchState(SharedConstants::SPLASH_STATE);
	}
	else
	{
		curState->SetView(renderWindow);
	}
}

void GameManager::Draw()
{
	if(curState != NULL)
	{
		curState->Draw(renderWindow);
		notificationBox.Render(renderWindow);
	}
}

void GameManager::SendDisconnectMessage()
{
	DisconnectionPacket disconnectionPacket;
	disconnectionPacket.PackData(SharedConstants::CONNECT_MODULE, serverNetworkData->playerID, disconnectionPacket);
	clientTransmitter->SendUDP(sharedConstants.GetClientTransmitPort(), serverNetworkData->serverIP, disconnectionPacket);
}
