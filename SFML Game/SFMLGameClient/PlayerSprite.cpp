#include "PlayerSprite.h"

PlayerSprite::PlayerSprite()
{
	facingDirection = RIGHT;

	healthBarSprite = new HealthBarSprite(GetPosition().x, GetPosition().y, 0, -10, sharedConstants.GetMaxHealth());
}

PlayerSprite::~PlayerSprite()
{
}

void PlayerSprite::SetLastMovementVector(float x, float y)
{
	lastMovementVector.x = x;
	lastMovementVector.y = y;
}

void PlayerSprite::Draw(sf::RenderWindow* renderWindow)
{
	renderWindow->Draw(*this);
	name.SetPosition(GetPosition().x + nameOffset.x, GetPosition().y + nameOffset.y);
	renderWindow->Draw(name);

	healthBarSprite->Draw(renderWindow, GetPosition().x, GetPosition().y);
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

void PlayerSprite::SetPlayerName(std::string playerName)
{
	this->name.SetText(playerName);
	this->name.SetScale(0.5f,0.5f);

	// Calculate the name offset used for rendering the nametag in the correct position
	nameOffset.x = static_cast<int>(-(name.GetRect().GetWidth())/2 + (this->GetSize().x / 2));
	nameOffset.y = -30;
}

sf::String PlayerSprite::GetPlayerName()
{
	return name;
}