#include "State.h"

State::State()
{
	ID = targetID = SharedConstants::UNDEFINED_SCREEN;
	loaded = false;
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
