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
	
	const std::string GetRequestMessage();
	const std::string GetAcceptMessage();
	const std::string GetRejectMessage();

	/** Contains all of the modules for the routers - could be split into server / client if need be. */
	enum modules { CONNECT_MODULE, GAME_MODULE, CHAT_MODULE };

	/** Contains all of the client game states - might be useful for server to know. */
	enum states { START_SCREEN, GAME_SCREEN };

private:
	SharedConstants();
	SharedConstants(SharedConstants const&); // Don't implement in .cpp
	void operator=(SharedConstants const&); // Don't implement in .cpp

	// Communication ports
	static const unsigned short serverReceivePort;
	static const unsigned short serverTransmitPort;
	static const unsigned short clientReceivePort;
	static const unsigned short clientTransmitPort;

	// Connection messages
	static const std::string requestMessage;
	static const std::string acceptMessage;
	static const std::string rejectMessage;
};
