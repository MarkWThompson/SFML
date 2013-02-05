#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "GameManager.h"


int main()
{
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

    // Create the main window, needs to be passed into drea
	sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "SFML Project");

    // Get a reference to the input manager associated to our window, and store it for later use
    const sf::Input& input = app.GetInput();

	GameManager* gameManager = new GameManager(app);

    // Start main loop
    while (app.IsOpened())
    {
        // Process events
        sf::Event events;
        while (app.GetEvent(events))
        {
            // Close window : exit
            if (events.Type == sf::Event::Closed)
			{
                app.Close();
			}

            // Escape key : exit
            if ((events.Type == sf::Event::KeyPressed) && (events.Key.Code == sf::Key::Escape))
			{
                app.Close();
			}

			// Update fsm with events and input
			gameManager->Update(events, input);
        }

		// Clear the screen (fill it with black color)
        app.Clear();

		// Draw any items from game manager
		gameManager->Draw(app);

		// Draw window
        app.Display();
    }

	// Clean up routine
	gameManager = NULL;
	delete gameManager;

    return EXIT_SUCCESS;
}
