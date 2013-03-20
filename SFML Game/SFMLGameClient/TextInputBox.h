#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

class TextInputBox
{
public:
	/** Default constructor. */
	TextInputBox();

	/**
	 * Parameterised constructor.
	 * @param x The top left x coordinate of the box.
	 * @param y The top left y coordinate of the box.
	 * @param boxWidth The width of the text input box.
	 * @param fontSize The size of the font.
	 * @param fontPath The location of the font.ttf file.
	 * @param maxChars The maximum number of chars that can fill the text input box - 255 by default.
	 * @param colour The colour of the font - black by default.
	 */
	TextInputBox(float x, float y, int boxWidth, float fontSize, std::string fontPath, int maxChars = 255, sf::Color colour = sf::Color(0, 0, 0));

	~TextInputBox();

	void Update(sf::Event &events, bool eventFired, const sf::Input &input);
	void Draw(sf::RenderWindow* renderWindow);
	void EraseBuffers();
	sf::Vector2f GetPosition();
	void SetPosition(float x, float y);

	// The text that is bing displayed in the chat window as you type
	sf::String currentTextDisplay; 

	// Used to store the entire message to send, before packing
	std::string sendBuffer;	

	sf::Rect<float> bounds;
	
private:
	// Loads in the font file
	void LoadFont(std::string fontPath);

	// Handle the inputting and aligning of text in the input box
	void HandleLocalTextInput(sf::Event &events, bool eventFired, const sf::Input &input);

	sf::Font font;

	// Used for adding text to the Chat buffer
	std::string tempBuffer; 

	// Set in setup as local co-ordinates, is the distance the textbox goes before is starts scrolling
	int rightTextBound; 

	// The maximum amount of chars in the textBox, set in setup
	int maxChars;

	bool shouldEnter;
};










//Your reward is the thought of the smell of cupcakes.