#include "Server.h"

Server::Server()
{
	// Initialise threads
	updateRunning = interactionRunning = true;
	
	playerNetworkData = new PlayerNetworkData();

	serverTransmitter = new ServerTransmitter();

	// Create server modules
	connectionHandler = new ConnectionHandler(serverTransmitter, playerNetworkData);
	gameLogic = new GameLogic(serverTransmitter, playerNetworkData);

	// Create server components
	serverRouter = new ServerRouter(connectionHandler, gameLogic);
	serverReceiver = new ServerReceiver(serverRouter);
}

Server::~Server()
{
	delete connectionHandler;
	delete gameLogic;
	delete playerNetworkData;
	delete serverTransmitter;
	delete serverReceiver;
	delete serverRouter;

	playerNetworkData = NULL;
	serverTransmitter = NULL;
	connectionHandler = NULL;
	gameLogic = NULL;
	serverRouter = NULL;
	serverReceiver = NULL;
}

void Server::Run()
{
	LoadLevel();

	// Start threads
	hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, &Server::UpdatePtr, this, 0, NULL);
	hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, &Server::InteractionPtr, this, 0, NULL);

	// Wait until the threads have finished execution
	WaitForMultipleObjects(NUM_THREADS, hThreads, TRUE, INFINITE);
}

unsigned __stdcall Server::Update()
{
	while(updateRunning == true)
	{
		serverReceiver->ReceiveUDP();
		connectionHandler->Update();
		gameLogic->Update();
	}

	return EXIT_SUCCESS;
}

void Server::LoadLevel()
{
	while(true) 
	{
		// Get user input
		std::string input;
		std::cout << "Enter level to run (press return for Default.xml): ";
		std::getline(std::cin, input);
		std::stringstream inputStream(input);
		
		// Set filepath of level
		std::string levelFilepath = "../Levels/";
		if(inputStream.str() == "")
		{
			inputStream << "Default";
		}
		levelFilepath += inputStream.str();
		this->levelName = inputStream.str();
		levelFilepath += ".xml"; // Add suffix which ensures type safety

		// Print file which will be tried to open
		std::cout << "Attempted to open: " << levelFilepath << std::endl;

		// Attempt to open xml file
		std::ifstream fileReader;
		fileReader.open(levelFilepath);

		if(!fileReader.good())
		{
			// Invalid input
			std::cout << "Could not load the file '" << inputStream.str() << "', please try again." << std::endl;
		}
		else
		{
			// Update modules with level information
			connectionHandler->SetLevelFilepath(levelFilepath);
			gameLogic->LoadLevel(levelFilepath);

			// Exit loop
			std::cout << "'" << inputStream.str() << ".xml' successfully loaded." << std::endl;
			std::cout << std::endl << "Skynet 2.0 now running..." << std::endl;
			break;
		}
	}
}

unsigned __stdcall Server::Interaction()
{
	std::cout << std::endl << "Type 'help' for a list of server commands." << std::endl;

	while(interactionRunning == true) 
	{
		// Get user input
		std::string input;
		std::getline(std::cin, input);
		std::stringstream inputStream(input);

		std::cout << std::endl; // Formatting

		// Prevent null input
		if(!input.empty())
		{
			if(input == HELP)
			{
				std::cout << "SERVER COMMANDS" << std::endl;
				std::cout << HELP << HELP_DESCRIPTION << std::endl;
				std::cout << INFO << INFO_DESCRIPTION << std::endl;
				std::cout << TERMINATE << TERMINATE_DESCRIPTION << std::endl;
				std::cout << PLAYERS << PLAYERS_DESCRIPTION << std::endl;
				std::cout << KICK << KICK_DESCRIPTION << std::endl;
			}
			else if(input == TERMINATE)
			{
				std::cout << "Server terminating..." << std::endl;
				updateRunning = interactionRunning = false; // Safely end threads
			}
			else if(input == PLAYERS)
			{
				std::cout << "PLAYER INFORMATION" << std::endl;
				for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
				{
					std::cout << "Player slot " << i << "\tIP: " << playerNetworkData->playerIPs[i] << "\tActive: " << playerNetworkData->playersActive[i] << std::endl;
				}
			}
			else if(input == INFO)
			{
				// Calculate number of active players
				int activePlayers = 0;
				for(int i = 0; i < playerNetworkData->MAX_PLAYERS; i++)
				{
					if(playerNetworkData->playersActive[i])
					{
						activePlayers++;
					}
				}
				
				std::cout << "SERVER INFO" << std::endl;
				std::cout << "Level : " << levelName << std::endl;
				std::cout << "Runtime: " << runtime.GetElapsedTime() << "s" << std::endl;
				std::cout << "Active players: " << activePlayers << "/" << playerNetworkData->MAX_PLAYERS << std::endl;
				std::cout << "Local IP : " << sf::IPAddress::GetLocalAddress() << std::endl;
				std::cout << "Public IP : " << sf::IPAddress::GetPublicAddress() << std::endl;
			}
			else if(input.find(KICK) != std::string::npos)
			{
				const int ID_POS = 5; // The position of the ID in the string 
				bool foundID = false; // Determines if the player ID was found in the string

				// Make sure the string actually has the capacity to store the ID, otherwise errors will be thrown
				if(input.length() >= ID_POS)
				{
					// Converts the input (ID) to an int
					std::stringstream ss(input.substr(ID_POS, ID_POS));
					int ID = -1;
					ss >> ID;

					if(ID != -1)
					{
						foundID = true;
						if(playerNetworkData->FreeSlot(ID))
						{
							std::cout << "Kicked player with ID : " << ID << std::endl;
						}
					}
				}
				
				if(!foundID)
				{
					std::cout << "Invalid kick command : use - 'kick #'." << std::endl;
				}
			}
			else
			{
				std::cout << "Unrecognised command." << std::endl;
			}
		}

		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}

unsigned __stdcall Server::UpdatePtr(void* ptr)
{
	return reinterpret_cast<Server*>(ptr)->Update();
}

unsigned __stdcall Server::InteractionPtr(void* ptr)
{
	return reinterpret_cast<Server*>(ptr)->Interaction();
}
