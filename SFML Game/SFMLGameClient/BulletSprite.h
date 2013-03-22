#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>

class BulletSprite : public sf::Sprite
{
public:
	BulletSprite(sf::Uint32 ID);
	~BulletSprite();

	

	void SetVelocity(float x, float y);
	sf::Vector2f GetVelocity();


	void SetLastMovement(float x, float y);
	sf::Vector2f GetLastMovement();

	/** Main update loop - called every frame. */
	void Update();

	/** Returns the active variable. */
	bool IsActive();

	sf::Uint32 GetID();
	
private:
	sf::Uint32 ID;


	sf::Vector2f lastMovement;

	/** Stores the x/y velocity components of the bullet. */
	sf::Vector2f velocity;

	/** Stores how long the bullet has been active for. */
	sf::Clock activeTime;

	/** Stores the time at which the bullet will no longer be active. */
	const float INACTIVE_TIME;

	/** If set to false, the bullet will no longer be updated and be removed from the application. */
	bool active;
};
