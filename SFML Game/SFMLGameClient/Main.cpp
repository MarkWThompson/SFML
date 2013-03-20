#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "GameManager.h"

int main()
{
	// Window dimensions
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	bool eventFired = false;

    // Create the main window, needs to be passed into drea
	sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "SFML Project");

    // Get a reference to the input manager associated to our window, and store it for later use
    const sf::Input& input = app.GetInput();

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

            // Escape key : exit
            if((events.Type == sf::Event::KeyPressed) && (events.Key.Code == sf::Key::Escape))
			{
				gameManager->SendDisconnectMessage();
                app.Close();
			}

			eventFired = true;
        }

		// Update fsm with events and input
		gameManager->Update(events, eventFired, input);

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
