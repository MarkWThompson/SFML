#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "State.h"
#include "Assets.h"
#include <iostream>
#include "Starfield.h"

class SplashState : public State
{
public:
	SplashState();

	~SplashState();

	/** 
	 * Loads state content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool Load();

	/** Main loop. */
	void Update(sf::Event events, bool eventFired, const sf::Input &input);

	/** Renders all of the applicable state content. */
	void Draw(sf::RenderWindow* renderWindow);

	// Not used in this state since it is not networked; can be though - leaving the possibility in for future.
	void SetView(sf::RenderWindow* renderWindow);
	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	sf::Image backgroundImage;
	sf::Sprite backgroundSprite;

	sf::Image messageImage;
	sf::Sprite messageSprite;

	Starfield starfield;

	const int FADE_AMOUNT;
	const int MIN_ALPHA;
	const int MAX_ALPHA;
	bool fadeMessage;

	void PulseMessage();
};
