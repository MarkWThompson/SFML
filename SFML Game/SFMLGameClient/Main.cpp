#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "GameManager.h"
#include "Assets.h"

int main()
{
	// Window dimensions
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	bool eventFired = false;
	
	sf::Randomizer::SetSeed((unsigned int)time(NULL));

    // Create the main window
	sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "M3");

	// Set window icon
	sf::Image icon;
	icon.LoadFromFile(ICON_IMAGE);
	app.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());

    // Get a reference to the input manager associated to our window, and store it for later use
    const sf::Input& INPUT = app.GetInput();

	GameManager* gameManager = new GameManager(&app);

    // Start main loop
    while(app.IsOpened())
    {
        // Process events
        sf::Event events;
        while(app.GetEvent(events))
        {
            // Close window : exit
            if(events.Type == sf::Event::Closed)
			{
				gameManager->SendDisconnectMessage();
                app.Close();
			}

			eventFired = true;
        }

		// Update fsm with events and input
		gameManager->Update(events, eventFired, INPUT);

		// Clear the screen
        app.Clear();

		// Draw any items from game manager
		gameManager->Draw();

		// Draw window
        app.Display();

		eventFired = false;
    }

	// Clean up routine
	gameManager = NULL;
	delete gameManager;

    return EXIT_SUCCESS;
}
