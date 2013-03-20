#include "PlayerSprite.h"

PlayerSprite::PlayerSprite()
{
	facingDirection = RIGHT;

	healthBarSprite = new HealthBarSprite(this->GetPosition().x, this->GetPosition().y, 0, -10, sharedConstants.GetMaxHealth());
}

PlayerSprite::~PlayerSprite()
{
	healthBarSprite = NULL;
	delete healthBarSprite;
}

void PlayerSprite::SetLastMovementVector(float x, float y)
{
	lastMovementVector.x = x;
	lastMovementVector.y = y;
}

void PlayerSprite::SetLastMovementVector(sf::Vector2f lastMovementVector)
{
	this->lastMovementVector = lastMovementVector;
}

sf::Vector2f PlayerSprite::GetLastMovementVector()
{
	return lastMovementVector;
}

void PlayerSprite::UpdateHealthBarHealth(int health)
{
	if(healthBarSprite != NULL)
	{
		healthBarSprite->UpdateHealth(health);
	}
}

void PlayerSprite::HandleOrientation()
{
	if((facingDirection == LEFT) && (lastMovementVector.x > 0))
	{
		facingDirection = RIGHT;
		FlipX(false);
	}
	else if((facingDirection == RIGHT) && (lastMovementVector.x < 0))
	{
		facingDirection = LEFT;
		FlipX(true);
	}
}

void PlayerSprite::DrawExtras(sf::RenderWindow &renderWindow)
{
	int xOffset = -(playerName.GetRect().GetWidth())/2 + (this->GetSize().x / 2);
	int yOffset = -30;
	playerName.SetPosition(GetPosition().x + xOffset, GetPosition().y + yOffset);
	renderWindow.Draw(this->playerName);

	healthBarSprite->Draw(renderWindow, GetPosition().x, GetPosition().y);
}

void PlayerSprite::SetPlayerName(std::string playerName)
{
	this->playerName.SetText(playerName);
	this->playerName.SetScale(0.5f,0.5f);
}

sf::String PlayerSprite::GetPlayerName()
{
	return playerName;
}