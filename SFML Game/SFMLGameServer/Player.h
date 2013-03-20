#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include "Assets.h"
#include "Object.h"
#include "../SharedConstants.h"

class Player : public Object
{
public:
	/** Default constructor. */
	Player();

	/** 
	 * Position setting constructor. 
	 * @param startX The initial x position of the player
	 * @param startY The intial y position of the player
	 */
	Player(float startX, float startY);

	/** Default destructor. */
	~Player();

	//Resets all the things
	void Reset();

	// Displaces the player position from where it is currently positioned
	void MoveDeltaX(float deltaX);
	void MoveDeltaY(float deltaY);
	void Move(float deltaX, float deltaY);

	// Sets the displacement variable
	void SetLastMovementVector(float x, float y);
	void SetLastMovementVector(sf::Vector2f lastMovementVector);

	// Sets the spawnPosition variable
	void SetSpawnPosition(float x, float y);
	void SetSpawnPosition(sf::Vector2f spawnPosition);

	/** Returns the y speed variable. */
	float GetYSpeed();

	/** Returns the x speed variable. */
	float GetXSpeed();

	/** Returns the position variable by reference. */
	sf::Vector2f& GetPositionRef();

	/** Returns the xSpeed and ySpeed variables compacted into a sf:Vector2f. */
	sf::Vector2f GetMoveSpeed();

	/** Returns the spawnPosition variable. */
	sf::Vector2f GetSpawnPosition();

	/** Returns the lastMovementVector variable. */
	sf::Vector2f GetLastMovementVector();

	/** Returns the projectileSpeed variable. */
	sf::Vector2f GetProjectileSpeed();

	Orientation GetFacingDirection();

	void SetHealth(int health);
	int GetHealth();

	/** Determines if the player can fire another projectile by comparing delay. */
	bool CanShoot();

	/** Handles collision with specified object. */
	void HandleCollision(sf::Rect<float> objectBounds);

	//This is the point on the player the bullets emerge from
	sf::Vector2f GetShootPosition();

	int GetScore();
	void SetScore(int score);

private:
	/** Sets the facingDirection variable depending on the displacement. */
	void DetermineOrientation();

	/** Stores the point at which the player will spawn. */
	sf::Vector2f spawnPosition;

	/** Stores the speed at which player projectiles travel at. */
	sf::Vector2f projectileSpeed;

	/** Stores the time since the last projectile was fired. */
	sf::Clock shootDelayClock;

	/** Stores the difference between current position and previous position. */
	sf::Vector2f lastMovementVector;

	/** Stores the delay between firing projectiles. */
	float shootDelay;

	float bulletSpawnXOffset;
	float bulletSpawnYOffset;

	int health;

	// Dimensions
	sf::Image* image;

	// Stores the speeds at which the player can move in each axis
	float xSpeed;
	float ySpeed;

	//The score!
	int score;
};
