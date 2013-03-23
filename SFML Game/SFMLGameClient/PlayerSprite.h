#pragma once

#include "SFML\Graphics.hpp"
#include <iostream>
#include "HealthBarSprite.h"
#include "SharedConstants.h"

class PlayerSprite : public sf::Sprite
{
public:
	PlayerSprite();

	~PlayerSprite();

	// Setters
	void SetLastMovementVector(float x, float y);
	void SetLastMovementVector(sf::Vector2f lastMovementVector);
	void SetPlayerName(std::string playerName);

	// Getters
	sf::String GetPlayerName();
	sf::Vector2f GetLastMovementVector();

	/** Enumerated type which determines the orientation of the player sprite. */
	enum Orientation { UP, DOWN, LEFT, RIGHT, UNDEFINED };

	/** Determines which way the player sprite is facing. */
	Orientation facingDirection;

	/** Sets the sprite face direction and flips accordingly. */
	void SetFacingDirection(Orientation direction);

	/** Handles the rendering of the player sprite and extra components like nametags and healthbars etc.*/
	void Draw(sf::RenderWindow* renderWindow);
	
	/** Scales the health bar based on the given health parameter. */
	void UpdateHealthBarHealth(int health);

private:
	/** Stores the difference between current position and previous position. */
	sf::Vector2f lastMovementVector;

	/** Stores the player name. */
	sf::String name;

	/** Stores the player name offset for rendering position. */
	sf::Vector2f nameOffset;

	/** Stores the health bar of the player. */
	HealthBarSprite *healthBarSprite;
};
