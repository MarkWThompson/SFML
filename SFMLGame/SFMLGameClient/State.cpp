#include "State.h"

State::State()
{
	ID = targetID = SharedConstants::UNDEFINED_STATE;
}

State::State(SharedConstants::StateID ID)
{
	this->ID = targetID = ID;
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
