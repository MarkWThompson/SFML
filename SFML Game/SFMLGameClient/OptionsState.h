#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "MenuState.h"
#include "Assets.h"
#include <string>
#include "InputBox.h"

class OptionsState : public MenuState
{
public:
	OptionsState();

	~OptionsState();

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

	void HandleGeneralOption(sf::Event events, bool eventFired, const sf::Input &INPUT);
	void HandleDisplayOption();
	void HandleAudioOption();

	enum Option { GENERAL, AUDIO, DISPLAY, UNDEFINED };

	Option curOption;

	// Button images
	sf::Image generalButtonImage;
	sf::Image audioButtonImage;
	sf::Image displayButtonImage;
	sf::Image changeButtonImage;

	// Buttons
	Button changeButton;
	Button generalButton;
	Button audioButton;
	Button displayButton;

	InputBox* usernameInputBox;


	// Username related
	const float MAX_MESSAGE_TIME;
	const int USERNAME_MAX_CHARS;
	sf::Clock messageTime;
	sf::String message;
	sf::Font font;
};
