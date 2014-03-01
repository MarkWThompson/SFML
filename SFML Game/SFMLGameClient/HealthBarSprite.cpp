#include "HealthBarSprite.h"


HealthBarSprite::HealthBarSprite(float playerCentreX, float playerCentreY, const int MAX_HEALTH)
	: MAX_HEALTH(MAX_HEALTH)
{
	backingImage.LoadFromFile(HEALTH_BAR_BACKING_IMAGE);
	fillImage.LoadFromFile(HEALTH_BAR_FILL_IMAGE);

	backingSprite.SetImage(backingImage);
	fillSprite.SetImage(fillImage);

	showFill = true;

	backingSprite.SetPosition(playerCentreX - 35, playerCentreY);
	fillSprite.SetPosition(playerCentreX - 35, playerCentreY);
}


HealthBarSprite::~HealthBarSprite()
{
}

void HealthBarSprite::UpdateHealth(int health)
{
	float scaleX = (float(health) / MAX_HEALTH);
	fillSprite.SetScaleX(scaleX);

	if(health <= 0)
	{
		showFill = false;
	}
	else
	{
		showFill = true;
	}
}

void HealthBarSprite::Draw(sf::RenderWindow* renderWindow, float xPos, float yPos)
{
	backingSprite.SetPosition(xPos - 35, yPos);
	fillSprite.SetPosition(xPos - 35, yPos);
	
	renderWindow->Draw(backingSprite);

	if(showFill == true)
	{
		renderWindow->Draw(fillSprite);
	}
}