#pragma once
#include <SFML\Graphics.hpp>
#include "Assets.h"
#include <iostream>

class HealthBarSprite
{
public:
	HealthBarSprite(float startX, float startY, int xOffset, int yOffset, float maxHealth);
	~HealthBarSprite(void);
	void Draw(sf::RenderWindow &renderWindow, float xPos, float yPos);
	void UpdateHealth(int health);
private:
	sf::Sprite healthBarFrame;
	sf::Sprite healthBarFill;
	sf::Image healthBarFrameImage;
	sf::Image healthBarFillImage;
	int xFillOffset;
	int yFillOffset;
	int fullXOffset;
	int fullYOffset;
	int maxHealth;
	bool showBar;

};

