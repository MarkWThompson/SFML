#pragma once

#include "StartState.h"
#include "GameState.h"
#include "ClientTransmitter.h"
#include "ClientReceiver.h"
#include "State.h"
#include "PacketTypes.h"
#include "SharedConstants.h"
#include "ServerNetworkData.h"

class GameManager
{
public:
	/** Default constructor. */
	GameManager();

	/** Default destructor. */
	~GameManager();

	// States
	StartState* startState;
	GameState* gameState;

	/** 
	 * Switches the curState to the specified target state.
	 * @param stateID The ID of the target state.
	 */
	void SwitchState(SharedConstants::StateID stateID);

	/** Main loop which updates the current state. */
	void Update(sf::Event events, bool eventFired, const sf::Input &input);

	/** Draws the current state. */
	void Draw(sf::RenderWindow &renderWindow);

	void SendDisconnectMessage();
private:
	//Stores the serverIP and the playerNum;
	ServerNetworkData* serverNetworkData;

	// I/O packet stream
	ClientTransmitter* clientTransmitter;
	ClientReceiver* clientReceiver;

	/** A pointer to the current state. */
	State* curState;
};
