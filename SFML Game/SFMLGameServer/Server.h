#pragma once

#include <SFML\Network.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "ServerCommands.h"
#include "ServerTransmitter.h"
#include "ServerReceiver.h"
#include "ServerRouter.h"
#include "ConnectionHandler.h"
#include "SharedConstants.h"
#include "PlayerNetworkData.h"
#include <process.h>
#include <sstream>

#define NUM_THREADS 2

class Server
{
public:
	/** Default constructor. */
	Server();

	/** Default destructor. */
	~Server();

	/** Main update loop. */
	void Run();

private:
	// Threads
	unsigned __stdcall Update();
	unsigned __stdcall Interaction();

	// Static thread pointer functions
	static unsigned __stdcall UpdatePtr(void* ptr);
	static unsigned __stdcall InteractionPtr(void* ptr);
	
	/** Handles the level loading. */
	void LoadLevel();
	
	/** Stores the handles to the threads. */
	HANDLE hThreads[NUM_THREADS];

	/** Determines if the update thread is running. */
	bool updateRunning;

	/** Determines if the interaction thread is running. */
	bool interactionRunning;

	PlayerNetworkData* playerNetworkData;	///< Stores player information
	
	// Modules
	ConnectionHandler* connectionHandler;	///< Handles player connections
	GameLogic* gameLogic;					///< Handles main gameplay logic

	// PIPES
	ServerTransmitter* serverTransmitter;	///< Used to transmit packets to target IP and port
	ServerRouter* serverRouter;				///< Routes received packets to designated modules of the server
	ServerReceiver* serverReceiver;			///< Receives incoming packets

	/** Stores the time the server has been running for in seconds. */
	sf::Clock runtime;

	/** Stores the name of the level that the server is hosting. */
	std::string levelName;
};