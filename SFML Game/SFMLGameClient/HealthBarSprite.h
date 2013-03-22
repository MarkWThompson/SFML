#pragma once
#include <SFML\Graphics.hpp>
#include "Assets.h"
#include <iostream>

class HealthBarSprite
{
public:
	/**
	 * Constructor for initialising the class' properties.
	 * @param startX The initial x position of the bar
	 * @param startY The initial y position of the bar
	 * @param xOffset The 
	 * @param yOffset
	 * @param MAX_HEALTH
	 */
	HealthBarSprite(float startX, float startY, const int X_OFFSET, const int Y_OFFSET, const int MAX_HEALTH);

	~HealthBarSprite();

	/** Renders the bar at the specified x and y location. */
	void Draw(sf::RenderWindow &renderWindow, float xPos, float yPos);

	/** Scales the fillSprite based on the health parameter. */
	void UpdateHealth(int health);

private:
	sf::Sprite frameSprite;
	sf::Sprite fillSprite;
	sf::Image frameImage;
	sf::Image fillImage;

	// Stores fillSprite specific offsets for rendering
	const int X_FILL_OFFSET;
	const int Y_FILL_OFFSET;

	// Stores the bar offsets for rendering
	const int BAR_X_OFFSET;
	const int BAR_Y_OFFSET;

	/** Stores the maximum possible health value of the player. */
	const int MAX_HEALTH;

	/** Determines whether or not the fillSprite variable is rendered in the Render function. */
	bool showFill;
};
