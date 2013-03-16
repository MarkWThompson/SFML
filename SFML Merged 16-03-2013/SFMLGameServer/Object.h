#pragma once
#include <SFML/Graphics.hpp>

class Object
{
public:
	/** Default constructor. */
	Object();

	/** Default destructor. */
	~Object();

	/** Enumerated type which determines the orientation of the object. */
	enum Orientation { UP, DOWN, LEFT, RIGHT, UNDEFINED };

	/** Returns the facingDirection variable. */
	Orientation GetFacingDirection();

	/** Determines if this object has collided with another object. */
	bool HitTestObject(sf::Rect<float> objectBounds); // I know this shouldn't be here

	/** Returns the position variable. */
	sf::Vector2f GetPosition();

	/** Returns the calculated width. */
	float GetWidth();

	/** Returns the calculated height. */
	float GetHeight();

	/** Returns the bounds variable. */
	sf::Rect<float> GetBounds();

	/** Sets the objects position. */
	void SetPosition(float x, float y);

protected:
	/** Stores the current position of the object. */
	sf::Vector2f position;

	// Object dimensions
	float height;
	float width;

	/** Determines which way the player is facing. */
	Orientation facingDirection;
};
