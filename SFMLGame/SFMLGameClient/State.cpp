#include "State.h"

State::State(SharedConstants::StateID ID)
{
	this->ID = targetID = ID;
	canReceive = false;
}

State::~State()
{
}

SharedConstants::StateID State::GetTarget()
{
	return targetID;
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

bool State::CanReceive()
{
	return canReceive;
}
