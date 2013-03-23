#include "Projectile.h"


Projectile::Projectile(float startX, float startY, sf::Vector2f velocity, sf::Vector2f dimensions, sf::Uint32 bulletID, int bulletOwner)
{
	SetBulletID(bulletID);

	position.x = startX;
	position.y = startY;

	this->velocity.x = velocity.x;
	this->velocity.y = velocity.y;
	
	bounds.Left = startX;
	bounds.Right = startX + dimensions.x;
	bounds.Top = startY;
	bounds.Bottom = startY + dimensions.y;

	this->bulletOwner = bulletOwner;
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{

}

void Projectile::Move(float x, float y)
{
	position.x += x;
	position.y += y;
	bounds.Offset(x,y);
}

void Projectile::SetBulletID(sf::Uint32 bulletID)
{
	this->bulletID = bulletID;
}

sf::Uint32 Projectile::GetBulletID()
{
	return bulletID;
}

sf::Rect<float> Projectile::GetBounds()
{
	return bounds;
}

void Projectile::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

sf::Vector2f Projectile::GetPosition()
{
	return sf::Vector2f(position.x,position.y);
}

sf::Vector2f Projectile::GetVelocity()
{
	return velocity;
}

void Projectile::SetBulletOwner(int bulletOwner)
{
	this->bulletOwner = bulletOwner;
}

int Projectile::GetBulletOwner()
{
	return bulletOwner;
}
