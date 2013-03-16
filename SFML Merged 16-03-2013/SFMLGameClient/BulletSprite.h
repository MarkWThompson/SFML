#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>

class BulletSprite : public sf::Sprite
{
public:
	BulletSprite(sf::Uint32 bulletID);
	~BulletSprite(void);

	sf::Uint32 bulletID;

	void SetVelocity(float x, float y);
	sf::Vector2f GetVelocity();

	void SetLastMovementVector(float x, float y);
	sf::Vector2f GetLastMovement();

	void update();
	bool pleaseErase;
private:
	//The last received movement vector
	sf::Vector2f lastMovement;
	sf::Vector2f velocity;
	sf::Clock lastResortTimeoutTimer;
	float timeoutTimerTimeoutTime; //Ha!
};

