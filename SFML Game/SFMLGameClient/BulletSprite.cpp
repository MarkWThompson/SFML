#include "BulletSprite.h"

BulletSprite::BulletSprite(sf::Uint32 ID) 
	: INACTIVE_TIME(60)
{
	this->ID = ID;
	activeTime.Reset();
	isActive = true;
}

BulletSprite::~BulletSprite()
{
}

sf::Uint32 BulletSprite::GetID()
{
	return ID;
}

bool BulletSprite::IsActive()
{
	return isActive;
}

void BulletSprite::Update()
{
	Move(velocity.x, velocity.y);

	if(activeTime.GetElapsedTime() > INACTIVE_TIME)
	{
		isActive = false;
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

void BulletSprite::SetLastMovement(float x, float y)
{
	lastMovement.x = x;
	lastMovement.y = y;
}

sf::Vector2f BulletSprite::GetLastMovement()
{
	return lastMovement;
}
