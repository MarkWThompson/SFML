#pragma once

#include "StartState.h"
#include "ClientTransmitter.h"
#include "ClientReceiver.h"
#include "ClientConnector.h"
#include "..\PacketTypes.h"
#include "State.h"
#include "../SharedConstants.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	StartState* startScreen;

	void SwitchState(SharedConstants::StateID stateID);
	void Update(sf::Event events, const sf::Input &input);
	void Draw(sf::RenderWindow &renderWindow);

private:
	ClientTransmitter* clientTransmitter;
	ClientReceiver* clientReceiver;
	ClientRouter* clientRouter;
	ClientConnector* clientConnector;

	State* curState;

};
