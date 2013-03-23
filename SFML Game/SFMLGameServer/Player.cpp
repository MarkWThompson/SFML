#include "Player.h"

Player::Player()
	: MAX_X_VELOCITY(10.0f), X_DECELERATION(1.0f), MIN_Y_VELOCITY(-20.0f), Y_ACCELERATION(-2.0), X_ACCELERATION(2.0f), JUMP_VELOCITY(25.0f)
{
	// Load in player image
	image = new sf::Image();
	if(!image->LoadFromFile(PLAYER_IMAGE))
	{
		std::cout << "Player::Player() error: Failed to load " << PLAYER_IMAGE << "." << std::endl;
	}
	else
	{
		// Grab dimensions of player image
		height = static_cast<float>(image->GetHeight());
		width = static_cast<float>(image->GetWidth());
		
		// No need to store this in memory anymore
		delete image;
		image = NULL;
	}

	gravityEnabled = true;
	isOnGround = false;
	isJumping = false;

	xVelocity = 0.0f;
	yVelocity = 0.0f;

	spawnPosition.x = 0.0f;
	spawnPosition.y = 0.0f;

	lastMovementVector.x = 0.0f;
	lastMovementVector.y = 0.0f;

	facingDirection = RIGHT;

	projectileSpeed.x = 20.0f;
	projectileSpeed.y = 0.0f;

	shootDelay = 0.2f;

	health = sharedConstants.GetMaxHealth();
	score = 0;

	isAlive = true;

	deathTime = 2.0f;
	deathTimer.Reset();
}

Player::~Player()
{
}

void Player::Reset()
{
	position = spawnPosition;

	bulletSpawnXOffset = 60.0f;
	bulletSpawnYOffset = 24.0f;

	xVelocity = 0.0f;
	yVelocity = 0.0f;

	gravityEnabled = true;
	isOnGround = false;
	isJumping = false;

	lastMovementVector.x = 0.0f;
	lastMovementVector.y = 0.0f;

	facingDirection = RIGHT;
	projectileSpeed.x = 20.0f;
	projectileSpeed.y = 0;

	shootDelay = 0.2f;

	health = sharedConstants.GetMaxHealth();

	isAlive = true;

	deathTime = 2.0f;
	deathTimer.Reset();
}

void Player::Update()
{
	if(movingHorizontally)
	{
		position.x += xVelocity;
	}
	else
	{
		Zero(xVelocity, X_DECELERATION);
	}

	position.y -= yVelocity;

	ApplyGravity();

	movingHorizontally = false;
	isOnGround = false;
}

void Player::Zero(float& velocity, const float DECELERATION)
{
	if(velocity != 0.0f)
	{
		if(velocity > 0.0f) // Positive velocity
		{
			velocity -= DECELERATION;

			if(velocity < 0.0f)
			{
				velocity = 0.0f;
			}
		}
		else if(velocity < 0.0f) // Negative velocity
		{
			velocity += DECELERATION;

			if(velocity > 0.0f)
			{
				velocity = 0.0f;
			}
		}
	}
}

void Player::ApplyGravity()
{
	// Enable gravity
	if((gravityEnabled == false) && (isJumping == false))
	{
		gravityEnabled = true;
		isOnGround = false;
	}

	// Apply downward force
	if(gravityEnabled == true)
	{
		SetVelocity(yVelocity, Y_ACCELERATION, MIN_Y_VELOCITY, JUMP_VELOCITY);
	}	
}

void Player::Jump()
{
	if((isJumping == false) && (isOnGround == true))
	{
		isJumping = true;
		yVelocity = JUMP_VELOCITY;
	}
}

void Player::MoveLeft()
{
	SetVelocity(xVelocity, -X_ACCELERATION, -MAX_X_VELOCITY, MAX_X_VELOCITY);
	movingHorizontally = true;
}

void Player::MoveRight()
{
	SetVelocity(xVelocity, X_ACCELERATION, -MAX_X_VELOCITY, MAX_X_VELOCITY);
	movingHorizontally = true;
}

bool Player::CanShoot()
{
	if(shootDelayClock.GetElapsedTime() > shootDelay)
	{
		shootDelayClock.Reset();
		return true;
	}
	else
	{
		return false;
	}
}

void Player::Move(float deltaX, float deltaY)
{
	position.x += deltaX;
	position.y += deltaY;
}

void Player::MoveDeltaX(float deltaX)
{
	position.x += deltaX;
}

void Player::MoveDeltaY(float deltaY)
{
	position.y += deltaY;
}

void Player::SetSpawnPosition(float x, float y)
{
	spawnPosition.x = x;
	spawnPosition.y = y;
}

void Player::SetSpawnPosition(sf::Vector2f spawnPosition)
{
	this->spawnPosition = spawnPosition;
}

void Player::SetLastMovementVector(float x, float y)
{
	lastMovementVector.x = x;
	lastMovementVector.y = y;

	DetermineOrientation();
}

void Player::SetLastMovementVector(sf::Vector2f lastMovementVector)
{
	this->lastMovementVector = lastMovementVector;

	DetermineOrientation();
}

void Player::DetermineOrientation()
{
	if(lastMovementVector.x > 0)
	{
		facingDirection = RIGHT;
	}
	else if(lastMovementVector.x < 0)
	{
		facingDirection = LEFT;
	}
}

float Player::GetYVelocity()
{
	return yVelocity;
}

float Player::GetXVelocity()
{
	return xVelocity;
}

sf::Vector2f Player::GetProjectileSpeed()
{
	return projectileSpeed;
}

sf::Vector2f& Player::GetPositionRef()
{
	return position;
}

sf::Vector2f Player::GetSpawnPosition()
{
	return spawnPosition;
}

sf::Vector2f Player::GetLastMovementVector()
{
	return lastMovementVector;
}

sf::Vector2f Player::GetShootPosition()
{
	sf::Vector2f shootPosition;
	
	if(facingDirection == RIGHT)
	{
		shootPosition.x = position.x + bulletSpawnXOffset;
	}
	else if(facingDirection == LEFT)
	{
		shootPosition.x = (position.x + (width/3)) - bulletSpawnXOffset;
	}

	shootPosition.y = position.y + bulletSpawnYOffset;

	return shootPosition;
}

void Player::SetVelocity(float& velocity, float acceleration, float minLimit, float maxLimit)
{
	velocity += acceleration;

	if(velocity > maxLimit)
	{
		velocity = maxLimit;
	}
	else if(velocity < minLimit)
	{
		velocity = minLimit;
	}
}

void Player::HandleCollision(sf::Rect<float> objectBounds)
{
	bool above = false;
    bool below = false;

    // Above object
	if((position.y + yVelocity + height) <= objectBounds.Top)
    {
		position.y = (objectBounds.Top - height);
        above = true;
		isJumping = false;
		isOnGround = true;
		yVelocity = 0.0f;
    }

    // Below object
	if((position.y - yVelocity) >= objectBounds.Bottom)
    {
		position.y = objectBounds.Bottom;
        below = true;
		isJumping = false;
    }

    if(!above && !below)
    {
        // Left of object
		if(position.x + xVelocity < objectBounds.Left)
        {
			position.x = (objectBounds.Left - width);
        }

        // Right of object
		if(position.x + xVelocity > objectBounds.Left)
        {
			position.x = objectBounds.Right;
        }
    }
}

int Player::GetHealth()
{
	return health;
}

void Player::SetHealth(int health)
{
	this->health = health;
}

void Player::IncreaseScore(int amount)
{
	this->score += amount;
}

int Player::GetScore()
{
	return score;
}

void Player::Die()
{
	isAlive = false;
	deathTimer.Reset();
}

bool Player::GetIsAlive()
{
	return isAlive;
}

void Player::SetIsAlive(bool isAlive)
{
	this->isAlive = isAlive;
}
