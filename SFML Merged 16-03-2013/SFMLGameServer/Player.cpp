#include "Player.h"

Player::Player()
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

	xSpeed = 5;
	ySpeed = 5;

	spawnPosition.x = 0;
	spawnPosition.y = 0;

	lastMovementVector.x = 0;
	lastMovementVector.y = 0;

	facingDirection = RIGHT;

	projectileSpeed.x = 15;
	projectileSpeed.y = 0;

	shootDelay = 0.2f;

	SetHealth(sharedConstants.GetMaxHealth());
	score = 0;
}

Player::~Player()
{
}

void Player::Reset()
{
	SetPosition(spawnPosition.x, spawnPosition.y);

	height = 68;
	width = 60;
	bulletSpawnXOffset = 60;
	bulletSpawnYOffset = 24;

	xSpeed = 7;
	ySpeed = 7;

	lastMovementVector.x = 0;
	lastMovementVector.y = 0;

	// Load in player image
	image = new sf::Image();
	if(!image->LoadFromFile(PLAYER_IMAGE))
	{
		std::cout << "Player::Player() error: Failed to load " << PLAYER_IMAGE << " while resetting" << std::endl;
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

	facingDirection = RIGHT;
	projectileSpeed.x = 15.0f;
	projectileSpeed.y = 0;

	shootDelay = 0.35f;

	SetHealth(sharedConstants.GetMaxHealth());
	score = 0;
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


float Player::GetYSpeed()
{
	return ySpeed;
}

float Player::GetXSpeed()
{
	return xSpeed;
}

sf::Vector2f Player::GetProjectileSpeed()
{
	return projectileSpeed;
}

sf::Vector2f& Player::GetPositionRef()
{
	return position;
}

sf::Vector2f Player::GetMoveSpeed()
{
	return sf::Vector2f(xSpeed, ySpeed);
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

int Player::GetFacingDirection()
{
	if(facingDirection == LEFT)
	{
		return -1;
	}
	else if(facingDirection == RIGHT)
	{
		return 1;
	}
	else
	{
		std::cout << ("Where the fuck are we facing? No one knows. Error in Player::GetFacing");
		return 0;
	}
}

void Player::HandleCollision(sf::Rect<float> objectBounds)
{
	bool above = false;
    bool below = false;

    // Above object
	if((position.y - ySpeed + height) <= objectBounds.Top)
    {
		position.y = (objectBounds.Top - height);
        above = true;
    }

    // Below object
	if((position.y + ySpeed) >= objectBounds.Bottom)
    {
		position.y = objectBounds.Bottom;
        below = true;
    }

    if(!above && !below)
    {
        // Left of object
		if(position.x - xSpeed < objectBounds.Left)
        {
			position.x = (objectBounds.Left - width);
        }

        // Right of object
		if(position.x - xSpeed > objectBounds.Left)
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

void Player::SetScore(int score)
{
	this->score = score;
}

int Player::GetScore()
{
	return score;
}