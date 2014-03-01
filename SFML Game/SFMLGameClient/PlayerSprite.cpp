#include "PlayerSprite.h"
#include "NotificationBox.h"

PlayerSprite::PlayerSprite()
	:	HEALTH_BAR_Y_OFFSET(15.0f), NAME_Y_OFFSET(5.0f), X_OFFSET(5.0f)
{
	facingDirection = RIGHT;
	prevFacingDirection = UNDEFINED;

	float centreX = GetPosition().x + (GetSize().x / 2) - X_OFFSET;
	float topY = GetPosition().y - HEALTH_BAR_Y_OFFSET;

	healthBarSprite = new HealthBarSprite(centreX, topY, sharedConstants.GetMaxHealth());

	fullImage = new sf::Image();

	// Load in player components
	if(!fullImage->LoadFromFile(PLAYER_IMAGE))
	{
		std::cout << "PlayerSprite::PlayerSprite() error: Failed to load " << PLAYER_HEAD_IMAGE << "." << std::endl;
		notificationBox.AddNotification("Fatal error: Failed to load player sprite.", true);
	}
	else
	{
		SetImage(*fullImage);
	}

	headImage = new sf::Image();
	armImage = new sf::Image();
	bodyImage = new sf::Image();

	if(!headImage->LoadFromFile(PLAYER_HEAD_IMAGE))
	{
		std::cout << "PlayerSprite::PlayerSprite() error: Failed to load " << PLAYER_HEAD_IMAGE << "." << std::endl;
		notificationBox.AddNotification("Fatal error: Failed to load player sprite.", true);
	}
	else
	{
		headSprite.SetImage(*headImage);
	}

	if(!armImage->LoadFromFile(PLAYER_ARM_IMAGE))
	{
		std::cout << "PlayerSprite::PlayerSprite() error: Failed to load " << PLAYER_ARM_IMAGE << "." << std::endl;
		notificationBox.AddNotification("Fatal error: Failed to load player sprite.", true);
	}
	else
	{
		armSprite.SetImage(*armImage);
	}

	if(!bodyImage->LoadFromFile(PLAYER_BODY_IMAGE))
	{
		std::cout << "PlayerSprite::PlayerSprite() error: Failed to load " << PLAYER_BODY_IMAGE << "." << std::endl;
		notificationBox.AddNotification("Fatal error: Failed to load player sprite.", true);
	}
	else
	{
		bodySprite.SetImage(*bodyImage);
	}

	// Initialise centres for rotation (RIGHT)
	headSprite.SetCenter(8.0f, 19.0f);
	armSprite.SetCenter(1.0f, 12.0f);
}

PlayerSprite::~PlayerSprite()
{
	//delete healthBarSprite;
	//healthBarSprite = NULL;
}

void PlayerSprite::SetLastMovementVector(float x, float y)
{
	lastMovementVector.x = x;
	lastMovementVector.y = y;
}

void PlayerSprite::Update(int playerHealth)
{
	UpdateHealthBar(playerHealth);
}

void PlayerSprite::UpdatePosition()
{
	float x = GetPosition().x;
	float y = GetPosition().y;

	float centreX = GetPosition().x + (GetSize().x / 2) - (X_OFFSET * 2);

	name.SetPosition(centreX - nameOffset.x, y + nameOffset.y);
	
	if(facingDirection == LEFT)
	{
		headSprite.SetPosition(x + 18.0f, y + 20.0f);
		armSprite.SetPosition(x + 9.0f, y + 25.0f);
		
	}
	else if(facingDirection == RIGHT)
	{
		headSprite.SetPosition(x + 20.0f, y + 19.0f);
		armSprite.SetPosition(x + 29.0f, y + 24.0f);
	}

	bodySprite.SetPosition(x, y + 11.0f);
}

void PlayerSprite::RotateComponents()
{
	sf::Vector2f armCentre;
	sf::Vector2f headCentre;

	armCentre.x = armSprite.GetPosition().x + (armSprite.GetSize().x / 2.0f);
	armCentre.y = armSprite.GetPosition().y + (armSprite.GetSize().y / 2.0f);
	headCentre.x = headSprite.GetPosition().x + (headSprite.GetSize().x / 2.0f);
	headCentre.y = headSprite.GetPosition().y + (headSprite.GetSize().y / 2.0f);

	float x2 = viewPoint.x;
	float y2 = viewPoint.y;

	float x1a = armCentre.x;
	float y1a = armCentre.y;
	float x1h = headCentre.x;
	float y1h = headCentre.y;

	float armAngle = atan2((y2-y1a), (x2-x1a)) * 57.2957795f;
	float headAngle = atan2((y2-y1h), (x2-x1h)) * 57.2957795f;

	armSprite.SetRotation(-armAngle);
	headSprite.SetRotation(-headAngle);
}

void PlayerSprite::Draw(sf::RenderWindow* renderWindow)
{
	UpdatePosition();
	RotateComponents();

	// The full body image is not rendered as it is for reference and collision
	renderWindow->Draw(armSprite);
	renderWindow->Draw(bodySprite);
	renderWindow->Draw(headSprite);

	renderWindow->Draw(name);

	float centreX = GetPosition().x + (GetSize().x / 2) - X_OFFSET;
	float topY = GetPosition().y - HEALTH_BAR_Y_OFFSET;

	healthBarSprite->Draw(renderWindow, centreX, topY);
}

void PlayerSprite::SetLastMovementVector(sf::Vector2f lastMovementVector)
{
	this->lastMovementVector = lastMovementVector;
}

sf::Vector2f PlayerSprite::GetLastMovementVector()
{
	return lastMovementVector;
}

void PlayerSprite::UpdateHealthBar(int health)
{
	if(healthBarSprite != NULL)
	{
		healthBarSprite->UpdateHealth(health);
	}
}

void PlayerSprite::SetFacingDirection(Orientation direction)
{
	prevFacingDirection = facingDirection;
	facingDirection = direction;

	if(facingDirection == LEFT)
	{
		armSprite.FlipY(true);
		headSprite.FlipY(true);
		bodySprite.FlipX(true);
		
		if(prevFacingDirection == RIGHT)
		{
			headSprite.SetCenter(8.0f, 3.0f);
			armSprite.SetCenter(1.0f, 11.0f);
		}
	}
	else if(facingDirection == RIGHT)
	{
		armSprite.FlipY(false);
		headSprite.FlipY(false);
		bodySprite.FlipX(false);
		
		if(prevFacingDirection == LEFT)
		{
			headSprite.SetCenter(8.0f, 19.0f);
			armSprite.SetCenter(1.0f, 12.0f);
		}
	}
}

void PlayerSprite::SetPlayerName(std::string playerName)
{
	this->name.SetText(playerName);
	this->name.SetScale(0.5f, 0.5f);

	// Calculate the name offset used for rendering the nametag in the correct position
	nameOffset.x = name.GetRect().GetWidth() / 2;
	nameOffset.y = -30 - NAME_Y_OFFSET;
}

sf::String PlayerSprite::GetPlayerName()
{
	return name;
}

void PlayerSprite::SetViewPoint(sf::Vector2f viewPoint)
{
	this->viewPoint = viewPoint;
}

sf::Vector2f PlayerSprite::GetViewPoint()
{
	return viewPoint;
}
