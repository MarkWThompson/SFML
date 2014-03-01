#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientTransmitter.h"
#include "InputBox.h"
#include "PacketTypes.h"
#include "ServerNetworkData.h"
#include "MenuState.h"
#include "Assets.h"

class LobbyState : public MenuState
{
public:
	/** 
		Constructor initialiser.
		@param clientTransmitter A pointer to the GameManager instance of the client transmitter.
		@param serverNetworkData A pointer to the server data storage location.
	*/
	LobbyState(ClientTransmitter* clientTransmitter, ServerNetworkData* serverNetworkData);

	~LobbyState();

	/** 
	 * Loads state content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool MenuLoad();

	/** Main loop. */
	void MenuUpdate(sf::Event events, bool eventFired, const sf::Input &INPUT);

	/** Renders all of the applicable state content. */
	void MenuDraw(sf::RenderWindow* renderWindow);

	void SetView(sf::RenderWindow* renderWindow);

	/** Receives a packet a processes its contents based upon its type. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

	/** Returns and then clears the text input box buffer. */
	sf::IPAddress ParseInputBox(InputBox *inputBox);

private:
	sf::Image connectButtonImage;
	Button connectButton;

	int notificationID;
	
	// Player image - Could be used for a player select, and is needed so the server knows about the player bounds
	sf::Image playerImage;

	/** Used to transmit packets from this state to the server. */
	ClientTransmitter *clientTransmitter;

	/** Stores information about the host server. */
	ServerNetworkData *serverNetworkData;

	/** The number of connection attempts to execute before exiting. */
	const int CONNECTION_ATTMEPTS;

	/** The time between a connection attempt. */
	const float CONNETION_TIME_INTERVAL;

	const float CONNECTION_LOAD_TIME;

	// Storage for received packet data
	int playerID;

	// Connection icon
	sf::Image connectionImage;
	sf::Sprite connectionIcon;

	InputBox* ipInputBox;
	bool ipInputBoxActive;

	/** Determines if the state is in the process of connecting to the server. */
	bool isConnecting;

	/** Determines if the state can connect to the server. */
	bool canConnect;

	/** Determines if the state should wait until receiving a response from the server. */
	bool listenForApproval;

	/** Determines if the state has connected to the server. */
	bool initialiseConnection;

	bool isConnected;

	// Used in the repeating connection attempt
	int connectionRequestIterator;
	sf::Clock connectionAttemptIntervalClock;
	sf::IPAddress candidateServerIP;

	sf::Clock connectionClock;
};
