#include "State.h"

State::State(SharedConstants::StateID ID)
{
	this->ID = targetID = ID;
	listening = false;
}

State::~State()
{
}

bool State::Switch()
{
	if(ID != targetID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void State::SwitchState(SharedConstants::StateID targetID)
{
	this->targetID = targetID;
}

SharedConstants::StateID State::GetTargetState()
{
	return targetID;
}

SharedConstants::StateID State::GetID()
{
	return ID;
}

void State::StartListening()
{
	listening = true;
}

void State::StopListening()
{
	listening = false;
}

bool State::IsListening()
{
	return listening;
}
