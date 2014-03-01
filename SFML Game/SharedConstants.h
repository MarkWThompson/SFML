#pragma once

#include <string>

// Macro for quickly grabbing the singleton instance
#define sharedConstants SharedConstants::GetInstance()

// For use with IP comparisons and such like
#define NULL_IP "0.0.0.0"

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

	/** Returns the TIMEOUT constant. */
	const float GetTimeout();

	/** Returns the TIMEOUT_INDICATION_TIME constant. */
	const float GetTimeoutIndicationTime();

	/** Gets Player Max Health */
	const int GetMaxHealth();

	const int GetWinningScore();

	const float GetGracePeriod();

	/** Contains all of the modules for the routers. */
	enum ModuleID { CONNECT_MODULE, GAME_MODULE, CHAT_MODULE };

	/** Contains all of the client game states. */
	enum StateID { UNDEFINED_STATE, SPLASH_STATE, GAME_STATE, LOBBY_STATE, CREDITS_STATE, OPTIONS_STATE, HELP_STATE };

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

	// Get the time before disconnecting will occur
	static const float TIMEOUT;

	// The time before displaying a error icon during game state when not receiving packets from server
	static const float TIMEOUT_INDICATION_TIME;

	// The maximum health value of a player
	static const int MAX_HEALTH;

	static const int WINNING_SCORE;

	static const float GRACE_PERIOD;
};
