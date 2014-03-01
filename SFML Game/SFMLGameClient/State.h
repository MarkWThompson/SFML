#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "SharedConstants.h"

class State
{
public:
	State(SharedConstants::StateID ID);

	virtual ~State();

	/** Abstract method intended to act as the main loop of the state.*/
	virtual void Update(sf::Event events, bool eventFired, const sf::Input &INPUT) = 0;

	/** Abstract method intended to render all objects of the state. */
	virtual void Draw(sf::RenderWindow* renderWindow) = 0;

	/**
	 * Abstract method intended to load state content.
	 * For instance: positioning objects, loading images, setting parameters.
	 * Returns true if the operation was successful, otherwise returns false.
	 */
	virtual bool Load() = 0;

	/** Abstract method intended to set the window view for the state. */
	virtual void SetView(sf::RenderWindow* renderWindow) = 0;

	/** Abstract method intended to receive packets. */
	virtual void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)  = 0;

	/** Returns true if targetID differs from ID, otherwise returns false. */
	bool Switch();

	// Getters
	SharedConstants::StateID GetTargetState();
	SharedConstants::StateID GetID();

	/** 
	 * Determines if the state is listening for packets.
	 * Returns the current state of the listening variable.
	 * This functionality is implemented in the FSM in gameManager::Update().
	 */
	bool IsListening();

protected:
	void SwitchState(SharedConstants::StateID targetID);

	void StartListening();
	void StopListening();

	/** Stores the window view of the state. */
	sf::View view;

private:
	/** Determines whether or not the state is listening for packets. */
	bool listening;

	/** The unique ID of the state. */
	SharedConstants::StateID ID;

	/** The unique ID of the desired state to switch to. */
	SharedConstants::StateID targetID;
};
