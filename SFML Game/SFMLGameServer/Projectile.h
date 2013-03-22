#pragma once

#include <SFML\Graphics.hpp>
#include "SharedConstants.h"

class Projectile
{
public:

	Projectile(float startX, float startY, sf::Vector2f velocity, sf::Vector2f dimensions, sf::Uint32 bulletID, int bulletOwner);
	~Projectile();

	void Update();
	void Move(float x, float y);
	void SetPosition(float x, float y);

	void SetBulletID(sf::Uint32 bulletID);
	sf::Uint32 GetBulletID();

	sf::Vector2f GetPosition();
	sf::Vector2f GetVelocity();

	std::vector<sf::Clock> lastNetworkSends;

	sf::Rect<float> GetBounds();

	int GetBulletOwner();
	void SetBulletOwner(int bulletOwner);

private:
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Uint32 bulletID;
	sf::Rect<float> bounds;
	int bulletOwner;
};

