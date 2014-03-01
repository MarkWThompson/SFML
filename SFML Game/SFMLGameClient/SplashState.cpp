#include "SplashState.h"
#include "AudioManager.h"
#include "SimplePersistence.h"

SplashState::SplashState()
	:	State(SharedConstants::SPLASH_STATE),
		FADE_AMOUNT(1), MIN_ALPHA(75), MAX_ALPHA(245)
{
	fadeMessage = true;

	if(!sPersistence.HasEntry("Username"))
	{
		sPersistence.SetString("Username", "Grunt");
	}
}

bool SplashState::Load()
{
	if(backgroundImage.LoadFromFile(SPLASH_BG_IMAGE))
	{
		backgroundSprite.SetImage(backgroundImage);
		backgroundSprite.SetPosition(0.0f, 0.0f);
	}
	else
	{
		return false;
	}

	if(messageImage.LoadFromFile(SPLASH_MESSAGE_IMAGE))
	{
		messageSprite.SetImage(messageImage);
		messageSprite.SetPosition(455.0f, 340.0f);
	}
	else
	{
		return false;
	}

	audioManager.gameMusic.Stop();
	audioManager.menuMusic.Play();

	return true;
}

SplashState::~SplashState()
{
}

void SplashState::Update(sf::Event events, bool eventFired, const sf::Input &input)
{
	// Handle input
	if(input.IsKeyDown(sf::Key::Return))
	{
		SwitchState(SharedConstants::LOBBY_STATE);
	}
	
	PulseMessage();

	// Update starfield
	starfield.Animate();
}

void SplashState::Draw(sf::RenderWindow* renderWindow)
{
	renderWindow->Draw(backgroundSprite);

	renderWindow->Draw(messageSprite);

	// Starfield
	starfield.Render(renderWindow);
}

void SplashState::SetView(sf::RenderWindow* renderWindow)
{
}

void SplashState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
}

void SplashState::PulseMessage()
{
	sf::Uint8 alpha = messageSprite.GetColor().a;

	if(fadeMessage)
	{
		if(alpha <= MIN_ALPHA)
		{
			fadeMessage = false;
		}

		alpha -= FADE_AMOUNT;
	}
	else
	{
		if(alpha >= MAX_ALPHA)
		{
			fadeMessage = true;
		}

		alpha += FADE_AMOUNT;
	}

	messageSprite.SetColor(sf::Color(255, 255, 255, alpha));
}
