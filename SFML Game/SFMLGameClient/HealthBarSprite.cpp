#include "HealthBarSprite.h"


HealthBarSprite::HealthBarSprite(float startX, float startY, const int X_OFFSET, const int Y_OFFSET, const int MAX_HEALTH)
	: MAX_HEALTH(MAX_HEALTH), X_FILL_OFFSET(3), Y_FILL_OFFSET(2), BAR_X_OFFSET(X_OFFSET), BAR_Y_OFFSET(Y_OFFSET)
{
	frameImage.LoadFromFile(HEALTHBARFRAME_IMAGE);
	fillImage.LoadFromFile(HEALTHBARFILL_IMAGE);

	frameSprite.SetImage(frameImage);
	fillSprite.SetImage(fillImage);

	showFill = true;

	frameSprite.SetPosition(startX + BAR_X_OFFSET, startY + BAR_Y_OFFSET);
	fillSprite.SetPosition(startX + BAR_X_OFFSET + X_FILL_OFFSET, startY + BAR_Y_OFFSET + Y_FILL_OFFSET);
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
	frameSprite.SetPosition(xPos + BAR_X_OFFSET, yPos + BAR_Y_OFFSET);
	fillSprite.SetPosition(xPos + BAR_X_OFFSET + X_FILL_OFFSET, yPos + BAR_Y_OFFSET + Y_FILL_OFFSET);
	
	if(showFill == true)
	{
		renderWindow->Draw(fillSprite);
	}

	renderWindow->Draw(frameSprite);
}