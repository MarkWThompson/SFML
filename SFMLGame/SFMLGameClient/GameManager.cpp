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

	startScreen = new StartScreen(renderWindow,"impact.ttf", 50.0f, 680); // The string probably needs to be a constant somewhere not in shared constants though

	clientRouter = new ClientRouter(clientConnector);
	listenForApproval = false;
}

GameManager::~GameManager()
{
	startScreen = NULL;
	delete startScreen;

	clientRouter = NULL;
	delete clientRouter;
}

void GameManager::Update(sf::Event events, const sf::Input &input)
{
	if(isOnStartScreen == true)
	{
		// This should probably be put somewhere else - requires the FSM
		startScreen->Update(events, input);

		if((input.IsKeyDown(sf::Key::Return)) && (canConnect == true))
		{
			canConnect = false;

			// Send connection request
			sf::Packet packetToSend;
			sf::Uint8 routingTagToSend = 0; // Need to talk about magic number packing / unpacking
			sf::Uint8 formatTagToSend = 0;

			packetToSend << routingTagToSend << formatTagToSend << sharedConstants.GetRequestMessage(); // Pack a connection packet, routing tag (To connector), format tag, (contains a connection packet), string data, (connection message) This should probably have its own function
			clientTransmitter.SendUDP(sharedConstants.GetClientTransmitPort(), startScreen->GetAndEraseIP(), packetToSend);
			listenForApproval = true;
		}
		if(listenForApproval == true)
		{
			clientReceiver.ReceiveUDP(sharedConstants.GetClientReceivePort(),*clientRouter,true);
			listenForApproval = false;
			if(clientConnector.isConnected == true)
			{
				isOnGame = true;
				isOnStartScreen = false;
			}

		}
		if(input.IsKeyDown(sf::Key::Return) == false)
		{
			//Dont delete or we connect too much when we hit enter
			canConnect = true;
		}

	}
	else if(isOnGame == true)
	{
		
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
