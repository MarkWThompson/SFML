#pragma once

#include "SpriteSheet.h"
#include <iostream>

/**
	BUTTON CLASS
	------------
	Created for Construct (OE coursework), modified for M3 (outdated SFML structure).
	Represents a generic command button.
	Reacts to most mouse events.
	Only supports 5 states - active, over, down, up, disabled.
	Fixed dimensions.
*/
class Button 
	:	public SpriteSheet
{
public:
	/**
		Default constructor.
		@param TRUE_BUTTON_WIDTH - The true width of the button image.
		@param TRUE_BUTTON_HEIGHT -The true height of the button image.
		@param BUTTON_WIDTH - The width of the button.
		@param BUTTON_HEIGHT - The height of the button.
	*/
	Button(const unsigned int TRUE_BUTTON_WIDTH, const unsigned int TRUE_BUTTON_HEIGHT, const unsigned int BUTTON_WIDTH, const unsigned int BUTTON_HEIGHT);

	~Button();

	/** Returns true if the mouse is over the button, otherwise returns false. */
	bool Over(const sf::Input &INPUT, sf::Vector2f view = DEFAULT_VIEW);

	/** Returns true if the mouse has been pressed and released over the button, otherwise returns false. */
	bool Up(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view = DEFAULT_VIEW);

	/** Returns true if the mouse is down over the button, otherwise returns false. */
	bool Down(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view = DEFAULT_VIEW);

	/** Returns true if the mouse has been pressed somewhere within the button region, otherwise returns flase. */
	bool Pressed(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view = DEFAULT_VIEW);

	/** The button will not longer be updated if this is called, thus disabling user interaction. */
	void Disable();

	/** The button will be updated if this is called, thus enabling user interaction. */
	void Enable();

	/** Updates the state of the button depending on the user interaction. */
	virtual void Update(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view = DEFAULT_VIEW);

protected:
	/** Unique IDs which determine the various states the button can be in. */
	enum ButtonState { BUTTON_ACTIVE, BUTTON_OVER, BUTTON_DOWN, BUTTON_UP, BUTTON_DISABLED };

	/** Will ignore user input when not active. */
	bool active;

	bool press;

	/** Holds the state of the button from the previous frame. */
	ButtonState prevState;

	static const sf::Vector2f DEFAULT_VIEW;
};
