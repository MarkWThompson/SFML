#pragma once
#include <string>

// Macro for quickly grabbing the singleton instance
#define sharedConstants SharedConstants::GetInstance()

// Singleton design pattern - global access, created once and safely destroyed
class SharedConstants
{
public:
	/** Grabs the singleton instance of the class. */
	static SharedConstants& GetInstance();

	// Returns the client and server receive / transmit ports
	const unsigned short GetServerReceivePort();
	const unsigned short GetServerTransmitPort();
	const unsigned short GetClientReceivePort();
	const unsigned short GetClientTransmitPort();
	
	// Returns the various connection messages
	const std::string GetRequestMessage();
	const std::string GetAcceptMessage();
	const std::string GetRejectMessage();

	/** Returns the TIME_STEP constant. */
	const float GetTimeStep();

	/** Contains all of the modules for the routers - could be split into server / client if need be. */
	enum ModuleID { CONNECT_MODULE, GAME_MODULE, CHAT_MODULE };

	/** Contains all of the client game states - might be useful for server to know. */
	enum StateID { UNDEFINED_STATE, START_STATE, GAME_STATE };

private:
	SharedConstants();
	SharedConstants(SharedConstants const&); // Don't implement in .cpp
	void operator=(SharedConstants const&); // Don't implement in .cpp

	// Communication ports
	static const unsigned short SERVER_RECEIVE_PORT;
	static const unsigned short SERVER_TRANSMIT_PORT;
	static const unsigned short CLIENT_RECEIVE_PORT;
	static const unsigned short CLIENT_TRANSMIT_PORT;

	// Connection messages
	static const std::string REQUEST_MESSAGE;
	static const std::string ACCEPT_MESSAGE;
	static const std::string REJECT_MESSAGE;

	// TimeStep
	static const float TIME_STEP;
};
