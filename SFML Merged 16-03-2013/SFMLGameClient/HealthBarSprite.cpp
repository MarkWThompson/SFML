#include "HealthBarSprite.h"


HealthBarSprite::HealthBarSprite(float startX, float startY, int xOffset, int yOffset, float maxHealth)
{
	this->maxHealth = maxHealth;

	healthBarFrameImage.LoadFromFile(HEALTHBARFRAME_IMAGE);
	healthBarFillImage.LoadFromFile(HEALTHBARFILL_IMAGE);

	healthBarFrame.SetImage(healthBarFrameImage);
	healthBarFill.SetImage(healthBarFillImage);

	fullXOffset = xOffset;
	fullYOffset = yOffset;
	
	xFillOffset = 3;
	yFillOffset = 2;

	healthBarFrame.SetPosition(startX + fullXOffset, startY + fullYOffset);
	healthBarFill.SetPosition(startX + fullXOffset + xFillOffset, startY + fullYOffset + yFillOffset);

}


HealthBarSprite::~HealthBarSprite(void)
{
}

void HealthBarSprite::UpdateHealth(int health)
{
	float scaleX = (float(health)/maxHealth);
	healthBarFill.SetScaleX(scaleX);

	if(health <= 0)
	{
		showBar = false;
	}
	else
	{
		showBar = true;
	}
}

void HealthBarSprite::Draw(sf::RenderWindow &renderWindow, float xPos, float yPos)
{
	healthBarFrame.SetPosition(xPos + fullXOffset, yPos + fullYOffset);
	healthBarFill.SetPosition(xPos + xFillOffset + fullXOffset, yPos + yFillOffset + fullYOffset);
	if(showBar == true)
	{
		renderWindow.Draw(healthBarFill);
	}
	renderWindow.Draw(healthBarFrame);
}