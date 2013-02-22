#include "SharedConstants.h"

// Define messages
const std::string SharedConstants::REQUEST_MESSAGE = "Requesting Connection";
const std::string SharedConstants::ACCEPT_MESSAGE = "Connection Accepted";
const std::string SharedConstants::REJECT_MESSAGE = "Connection Rejected";

// Define port numbers
const unsigned short SharedConstants::SERVER_RECEIVE_PORT = 4567;
const unsigned short SharedConstants::SERVER_TRANSMIT_PORT = 3242;
const unsigned short SharedConstants::CLIENT_RECEIVE_PORT = 3242;
const unsigned short SharedConstants::CLIENT_TRANSMIT_PORT = 4567;

// Define the fixed time step
const float SharedConstants::TIME_STEP = 33.3f / 1000.0f; //this is 33.2 seconds / 1000, or 16.6ms, or 30fps. Far too high to be honest. This is the amount of time the gameLogic update function waits before executing again.

// Define the timeout duration
const float SharedConstants::TIMEOUT = 6.0f;

const float SharedConstants::TIMEOUT_INDICATION_TIME = 5.0f;

SharedConstants::SharedConstants()
{
}

SharedConstants& SharedConstants::GetInstance()
{
	// Instantiated on first use, reference passed thereafter
	static SharedConstants instance;
	return instance;
}

const unsigned short SharedConstants::GetServerReceivePort()
{
	return SERVER_RECEIVE_PORT;
}

const unsigned short SharedConstants::GetServerTransmitPort()
{
	return SERVER_TRANSMIT_PORT;
}

const unsigned short SharedConstants::GetClientReceivePort()
{
	return CLIENT_RECEIVE_PORT;
}

const unsigned short SharedConstants::GetClientTransmitPort()
{
	return CLIENT_TRANSMIT_PORT;
}

const std::string SharedConstants::GetRequestMessage()
{
	return REQUEST_MESSAGE;
}

const std::string SharedConstants::GetAcceptMessage()
{
	return ACCEPT_MESSAGE;
}

const std::string SharedConstants::GetRejectMessage()
{
	return REJECT_MESSAGE;
}

const float SharedConstants::GetTimeStep()
{
	return TIME_STEP;
}

const float SharedConstants::GetTimeout()
{
	return TIMEOUT;
}

const float SharedConstants::GetTimeoutIndicationTime()
{
	return TIMEOUT_INDICATION_TIME;
}
