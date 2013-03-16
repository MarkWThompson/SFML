#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include "HealthBarSprite.h"
#include "../SharedConstants.h"

class PlayerSprite : public sf::Sprite
{
public:
	/** Default constructor. */
	PlayerSprite();

	/** Default destructor. */
	~PlayerSprite();

	/** Sets the lastMovementVector variable with the specified components. */
	void SetLastMovementVector(float x, float y);

	/** Sets the lastMovementVector variable with a sf::Vector2f. */
	void SetLastMovementVector(sf::Vector2f lastMovementVector);

	/** Returns the lastMovementVector variable. */
	sf::Vector2f GetLastMovementVector();

	/** Enumerated type which determines the orientation of the player sprite. */
	enum Orientation {LEFT, RIGHT};

	/** Determines which way the player sprite is facing. */
	Orientation facingDirection;

	/** Handles the flipping of the player sprite based on its displacement. */
	void HandleOrientation();

	//This draws things like healthbars and nametags and whatever
	void DrawExtras(sf::RenderWindow &renderWindow);
	void SetPlayerName(std::string playerName);
	void UpdateHealthBarHealth(int health);

private:
	/** Stores the difference between current position and previous position. */
	sf::Vector2f lastMovementVector;
	sf::String playerName;
	HealthBarSprite *healthBarSprite;
};
