#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "AudioManager.h"
#include "State.h"
#include "Assets.h"
#include "Button.h"
#include "Starfield.h"

class MenuState : public State
{
public:
	MenuState(SharedConstants::StateID ID);

	virtual ~MenuState();

	/** 
	 * Loads state content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool Load();

	/** Main loop. */
	void Update(sf::Event events, bool eventFired, const sf::Input &INPUT);

	/** Renders all of the applicable state content. */
	void Draw(sf::RenderWindow* renderWindow);

	/** 
		Abstract functions intended to define specific subclass menu behaviour(s).
		These are called in the respective functions listed above.
	*/
	virtual bool MenuLoad() = 0;
	virtual void MenuUpdate(sf::Event events, bool eventFired, const sf::Input &input) = 0;
	virtual void MenuDraw(sf::RenderWindow* renderWindow) = 0;

	// Redefine base abstract functions for subclass definitions.
	virtual void SetView(sf::RenderWindow* renderWindow) = 0;
	virtual void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port) = 0;

protected:
	sf::Image overlayImage;
	sf::Sprite overlaySprite;

private:
	Starfield starfield;

	sf::Image backgroundImage;
	sf::Sprite backgroundSprite;

	// Button images
	sf::Image lobbyButtonImage;
	sf::Image optionsButtonImage;
	sf::Image helpButtonImage;
	sf::Image creditsButtonImage;

	// Buttons
	Button lobbyButton;
	Button optionsButton;
	Button helpButton;
	Button creditsButton;
};
