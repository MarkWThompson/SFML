#include "BulletSprite.h"


BulletSprite::BulletSprite(sf::Uint32 bulletID)
{
	this->bulletID = bulletID;
	timeoutTimerTimeoutTime = 60;
	lastResortTimeoutTimer.Reset();
	pleaseErase = false;
}


BulletSprite::~BulletSprite(void)
{
}

void BulletSprite::update()
{
	Move(velocity.x, velocity.y);

	if(lastResortTimeoutTimer.GetElapsedTime() > timeoutTimerTimeoutTime)
	{
		pleaseErase = true;
	}
}

void BulletSprite::SetVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;

	if(velocity.x >= 0)
	{
		FlipX(false);
	}
	else if(velocity.x < 0)
	{
		FlipX(true);
	}
}

sf::Vector2f BulletSprite::GetVelocity()
{
	return velocity;
}

void BulletSprite::SetLastMovementVector(float x, float y)
{
	lastMovement.x = x;
	lastMovement.y = y;
}

sf::Vector2f BulletSprite::GetLastMovement()
{
	return sf::Vector2f(lastMovement.x, lastMovement.y);
}
