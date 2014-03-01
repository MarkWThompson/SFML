#pragma once

#include "SFML\Graphics.hpp"
#include <iostream>
#include "HealthBarSprite.h"
#include "SharedConstants.h"
#include "Assets.h"

class PlayerSprite : public sf::Sprite
{
public:
	PlayerSprite();

	~PlayerSprite();

	// Setters
	void SetLastMovementVector(float x, float y);
	void SetLastMovementVector(sf::Vector2f lastMovementVector);
	void SetPlayerName(std::string playerName);
	void SetViewPoint(sf::Vector2f viewPoint);

	// Getters
	sf::String GetPlayerName();
	sf::Vector2f GetLastMovementVector();
	sf::Vector2f GetViewPoint();

	/** Enumerated type which determines the orientation of the player sprite. */
	enum Orientation { UP, DOWN, LEFT, RIGHT, UNDEFINED };

	/** Determines which way the player sprite is facing. */
	Orientation facingDirection;

	/** Sets the sprite face direction and flips accordingly. */
	void SetFacingDirection(Orientation direction);

	/** Handles the rendering of the player sprite and extra components like nametags and healthbars etc.*/
	void Draw(sf::RenderWindow* renderWindow);

	void Update(int playerHealth);
	

private:
	const float HEALTH_BAR_Y_OFFSET;
	const float NAME_Y_OFFSET;

	const float X_OFFSET;

	void RotateComponents();

	/** Scales the health bar based on the given health parameter. */
	void UpdateHealthBar(int playerHealth);

	void UpdatePosition();

	Orientation prevFacingDirection;

	/** Stores the difference between current position and previous position. */
	sf::Vector2f lastMovementVector;

	/** Stores the player name. */
	sf::String name;

	/** Stores the player name offset for rendering position. */
	sf::Vector2f nameOffset;

	sf::Vector2f viewPoint;

	/** Stores the health bar of the player. */
	HealthBarSprite *healthBarSprite;

	sf::Image* fullImage;
	sf::Image* headImage;
	sf::Image* bodyImage;
	sf::Image* armImage;

	sf::Sprite fullSprite;
	sf::Sprite headSprite;
	sf::Sprite bodySprite;
	sf::Sprite armSprite;
};
