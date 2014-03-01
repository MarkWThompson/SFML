#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

class InputBox
{
public:
	/**
		Parameterised constructor.
		@param x The top left x coordinate of the box.
		@param y The top left y coordinate of the box.
		@param boxWidth The width of the backing box.
		@param boxHeight The height of the backing box.
		@param fontSize The size of the font.
		@param FONT_FILEPATH The (relative) filepath of the font.ttf file.
		@param maxChars The maximum number of chars that can fill the text input box - 255 by default.
		@param colour The colour of the font - black by default.
	*/
	InputBox(float x, float y, int boxWidth, int boxHeight, int inputWidth, int padding, float fontSize, const std::string FONT_FILEPATH, int maxChars = 255, sf::Color colour = sf::Color(0, 0, 0));

	~InputBox();

	// Handle the inputting and aligning of text in the input box
	void Update(sf::Event &events, bool eventFired, const sf::Input &input);

	// Draw the backing and text
	void Draw(sf::RenderWindow* renderWindow);

	void EraseBuffers();

	/** Returns the top left position of the input box. */
	sf::Vector2f GetPosition();

	/** Sets the position of the input box. */
	void SetPosition(float x, float y);

	void SetText(std::string text);

	/** The text that is being displayed to the user. */
	sf::String curTextDisplay; 

	std::string GetInput();
	
private:
	/** Removes letters from the visible buffer until it is of the correct size. */
	void Fit();

	/** Loads in the font file. */
	bool LoadFont(const std::string FONT_FILEPATH);

	/** Represents the backing box. */
	sf::Shape backing;

	/** The input box font. */
	sf::Font font;

	/** Stores visible part of the string. */
	std::string visibleBuffer; 

	/** Stores the entire inputted string which may or may not be visible to user as scrolling may have occurred. */
	std::string entireBuffer;	

	/** 
		The distance the text will display until scrolling will start. 
		In local coords. 
	*/
	int writingBound; 

	/** The maximum amount of chars in the textBox. */
	int maxChars;

	/** The padding applied internally to the position of the text. */
	int padding;

	int inputWidth;
};
