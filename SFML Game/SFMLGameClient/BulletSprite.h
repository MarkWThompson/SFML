#pragma once

#include "SFML\Graphics.hpp"
#include <iostream>

class BulletSprite : public sf::Sprite
{
public:
	BulletSprite(sf::Uint32 ID);
	~BulletSprite();

	// Setters
	void SetVelocity(float x, float y);
	void SetLastMovement(float x, float y);

	// Getters
	sf::Uint32 GetID();
	sf::Vector2f GetVelocity();
	sf::Vector2f GetLastMovement();
	bool IsActive();

	/** Main update loop - called every frame. */
	void Update();
	
private:
	/** Stores the bullets unique ID. */
	sf::Uint32 ID;

	/** Stores the difference between current position and previous position. */
	sf::Vector2f lastMovement;

	/** Stores the x/y velocity components of the bullet. */
	sf::Vector2f velocity;

	/** Stores how long the bullet has been active for. */
	sf::Clock activeTime;

	/** Stores the time at which the bullet will no longer be active. */
	const float INACTIVE_TIME;

	/** If set to false, the bullet will no longer be updated and be removed from the application. */
	bool isActive;

};
