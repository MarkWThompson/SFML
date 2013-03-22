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

	~Player();

	/** Resets all variables. */
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
	float GetYVelocity();

	/** Returns the x speed variable. */
	float GetXVelocity();

	/** Returns the position variable by reference. */
	sf::Vector2f& GetPositionRef();

	/** Returns the spawnPosition variable. */
	sf::Vector2f GetSpawnPosition();

	/** Returns the lastMovementVector variable. */
	sf::Vector2f GetLastMovementVector();

	/** Returns the projectileSpeed variable. */
	sf::Vector2f GetProjectileSpeed();

	void Update();
	void MoveLeft();
	void MoveRight();
	void Jump();

	// Health G/S
	void SetHealth(int health);
	int GetHealth();

	/** Determines if the player can fire another projectile by comparing delay. */
	bool CanShoot();

	/** Handles collision with specified object. */
	void HandleCollision(sf::Rect<float> objectBounds);

	//This is the point on the player the bullets emerge from
	sf::Vector2f GetShootPosition();

	// Score G/S
	int GetScore();
	void IncreaseScore(int amount);

	void Die();
	bool GetIsAlive();
	void SetIsAlive(bool isAlive);
	
	//The death timer
	sf::Clock deathTimer;
	float deathTime;

private:
	// X movement
	float xVelocity;
	const float X_ACCELERATION;
	const float X_DECELERATION;
	const float MAX_X_VELOCITY;
	bool movingHorizontally;

	// Y movement
	const float MIN_Y_VELOCITY;
	const float JUMP_VELOCITY;
	const float Y_ACCELERATION;
	float yVelocity;

	bool isJumping;
	bool isOnGround;
	bool gravityEnabled;

	void ApplyGravity();
	void Zero(float& velocity, const float DECELERATION);

	void SetVelocity(float& velocity, float acceleration, float minLimit, float maxLimit);

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

	/** Stores the players current health. */
	int health;

	// Dimensions
	sf::Image* image;

	/** Stores the players current score. */
	int score;

	//Determines whether a player is alive
	bool isAlive;
};
