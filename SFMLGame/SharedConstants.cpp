#include "SharedConstants.h"


// Define messages
const std::string SharedConstants::requestMessage = "Requesting Connection";
const std::string SharedConstants::acceptMessage = "Connection Accepted";
const std::string SharedConstants::rejectMessage = "Connection Rejected";

// Define port numbers
const unsigned short SharedConstants::serverReceivePort = 4567;
const unsigned short SharedConstants::serverTransmitPort = 3242;
const unsigned short SharedConstants::clientReceivePort = 3242;
const unsigned short SharedConstants::clientTransmitPort = 4567;

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
	return serverReceivePort;
}

const unsigned short SharedConstants::GetServerTransmitPort()
{
	return serverTransmitPort;
}

const unsigned short SharedConstants::GetClientReceivePort()
{
	return clientReceivePort;
}

const unsigned short SharedConstants::GetClientTransmitPort()
{
	return clientTransmitPort;
}

const std::string SharedConstants::GetRequestMessage()
{
	return requestMessage;
}

const std::string SharedConstants::GetAcceptMessage()
{
	return acceptMessage;
}

const std::string SharedConstants::GetRejectMessage()
{
	return rejectMessage;
}
