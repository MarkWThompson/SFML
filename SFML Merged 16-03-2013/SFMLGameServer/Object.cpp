#include "Object.h"

Object::Object()
{
	height = width = 0.0f;

	position = sf::Vector2f(0.0f, 0.0f);

	facingDirection = UNDEFINED;
}

Object::~Object()
{
}

bool Object::HitTestObject(sf::Rect<float> objectBounds)
{
	if((position.x + width) < objectBounds.Left)
	{
		return false;
	}
	if(position.x > objectBounds.Right)
	{
		return false;
	}
	if((position.y + height) < objectBounds.Top)
	{
		return false;
	}
	if(position.y > objectBounds.Bottom)
	{
		return false;
	}

	return true;
}

void Object::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

sf::Rect<float> Object::GetBounds()
{
	sf::Rect<float> bounds = sf::Rect<float>(position.x, position.y, position.x + width, position.y + height);
	return bounds;
}

float Object::GetWidth()
{
	return width;
}

float Object::GetHeight()
{
	return height;
}

sf::Vector2f Object::GetPosition()
{
	return position;
}

\