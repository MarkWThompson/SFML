#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientTransmitter.h"
#include "TextInputBox.h"
#include <iostream>
#include "..\PacketTypes.h"
#include "State.h"

class StartState : public State
{
public:
	/** 
	 * Constructor initialiser.
	 * @param clientTransmitter A pointer to the client transmitter.
	 * @param ipFontPath The filepath of the font file used to represent the text.
	 * @param fontSize The fontSize of the text input box.
	 * @param boxWidth The width of the text input box.
	 */
	StartState(ClientTransmitter* clientTransmitter, std::string ipFontPath, float fontSize, int boxWidth);

	/** Default destructor. */
	~StartState();

	/** 
	 * Loads StatrState content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool Load();

	/** Main loop. */
	void Update(sf::Event events, const sf::Input &input);

	/** Renders all of the applicable state content. */
	void Draw(sf::RenderWindow &renderWindow);

	/** Receives a packet a processes its contents based upon its type. */
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

	/** Returns and then clears the text input box buffer. */
	sf::IPAddress GetAndEraseIP();

	/** 
	 * Returns the isConnected variable.
	 * Determines if the client has actually connected to the server.
	 */
	bool IsConnected();

private:
	/** Used to transmit packets from this state to the server. */
	ClientTransmitter *clientTransmitter;

	// Storage for received packet data
	int playerID;
	sf::IPAddress serverIP;
	unsigned short serverPort;

	// Background image
	sf::Image startScreenImage;
	sf::Sprite startScreen;

	/** Used to listen for user input and store the data typed. */
	TextInputBox* ipInput;

	/** Determines if the state is in the process of connecting to the server. */
	bool isConnecting;

	/** Determines if the state can connect to the server. */
	bool canConnect;

	/** Determines if the state should wait until receiving a response from the server. */
	bool listenForApproval;

	/** Determines if the state has connected to the server. */
	bool isConnected;
};
