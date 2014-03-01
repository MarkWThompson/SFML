#pragma once

#include <SFML\Graphics.hpp>
#include <math.h>

/** 
	SPRITE SHEET CLASS
	------------------
	This class provides functionality for sprites which have multiple frames.
	Supports consitently timed animation.
*/ 
class SpriteSheet 
	:	public sf::Sprite
{
public:
	/**
		Default constructor - sets the image of the sprite sheet on construction.
		@param image - The sprite sheet to be used.
		@param numFrames - The number of frames the sprite sheet contains.
		@param FRAME_WIDTH - The width of an individual frame.
		@param FRAME_HEIGHT	- The height of an individual frame.
		@param curFrame Sets	- The current frame of the sprite sheet.
	*/
	SpriteSheet(sf::Image& image, unsigned int numFrames, const unsigned int FRAME_WIDTH, const unsigned int FRAME_HEIGHT, unsigned int curFrame = 0);

	/**
		Lazy constructor - no need to set the image immediately.
		@param trueWidth	- The width of the sprite sheet which will be set.
		@param trueHeight - The height of the sprite sheet which will be set.
		@param numFrames	- The number of frames the sprite sheet contains.
		@param FRAME_WIDTH - The width of an individual frame.
		@param FRAME_HEIGHT - The height of an individual frame.
		@param curFrame - Sets the current frame of the sprite sheet.
	*/
	SpriteSheet(unsigned int trueWidth, unsigned int trueHeight, unsigned int numFrames, const unsigned int FRAME_WIDTH, const unsigned int FRAME_HEIGHT, unsigned int curFrame = 0);

	~SpriteSheet();

	void SetFrame(unsigned int frame);

protected:
	/** Sets common properties which each of the constructors requires. */
	void Init();

	sf::IntRect GetBounds();

	// Dimensions of the subrect
	const unsigned int HEIGHT_;	///< The height of an individual frame
	const unsigned int WIDTH_;	///< The width of an individual frame

	// Dimensions of the spritesheet
	unsigned int trueWidth_;	///< The actual width of the sprite sheet
	unsigned int trueHeight_;	///< The actual height of the sprite sheet

	// Rendering data
	unsigned int numFrames_;	///< The total of number of frames that fill the sprite sheet.
	unsigned int numRows_;		///< The number of rows that make up the sprite sheet.
	unsigned int numColumns_;	///< The number of columns that make up the sprite sheet.
	unsigned int curFrame_;		///< The current frame of the sprite sheet being rendered.
	unsigned int curColumn_;	///< The current sprite sheet column being rendered.
	unsigned int curRow_;		///< The current sprite sheet row being rendered.
	unsigned int newRow_;		///< Determines what frame is the start of a new row in the sprite sheet.

private:
	/** 
		Stores the bounds of the sprite in screen space - not relative to the sprite sheet. 
		e.g.	left = sprite x pos on screen, 
				top = sprite y pos on screen,
				right = left + frame width,
				bottom = top + frame height
	*/
	sf::IntRect bounds_;
};
