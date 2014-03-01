#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "MenuState.h"
#include "Assets.h"

class CreditsState : public MenuState
{
public:
	CreditsState();

	~CreditsState();

	/** 
	 * Loads state content.
	 * Returns true if successful, otherwise returns false.
	 */
	bool MenuLoad();

	/** Main loop. */
	void MenuUpdate(sf::Event events, bool eventFired, const sf::Input &INPUT);

	/** Renders all of the applicable state content. */
	void MenuDraw(sf::RenderWindow* renderWindow);

	void SetView(sf::RenderWindow* renderWindow);

	void ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port);

private:
	sf::Font font;
	sf::String stateName;
};
