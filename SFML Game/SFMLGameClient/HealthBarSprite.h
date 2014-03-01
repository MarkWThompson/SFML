#pragma once

#include <SFML\Graphics.hpp>
#include "Assets.h"
#include <iostream>

class HealthBarSprite
{
public:
	/**
	 * Constructor for initialising the class' properties.
	 * @param playerCentreX The centre x position of the player
	 * @param playerTopY The topmost y position of the player
	 * @param MAX_HEALTH The maximum health value for use with scaling.
	 */
	HealthBarSprite(float playerCentreX, float playerTopY, const int MAX_HEALTH);

	~HealthBarSprite();

	/** Renders the bar at the specified x and y location. */
	void Draw(sf::RenderWindow* renderWindow, float xPos, float yPos);

	/** Scales the fillSprite based on the health parameter. */
	void UpdateHealth(int health);

private:
	sf::Sprite backingSprite;
	sf::Sprite fillSprite;

	sf::Image backingImage;
	sf::Image fillImage;

	/** Stores the maximum possible health value of the player. */
	const int MAX_HEALTH;

	/** Determines whether or not the fillSprite variable is rendered. */
	bool showFill;
};
