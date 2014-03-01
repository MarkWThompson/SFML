#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(sf::Image& image, unsigned int numFrames, const unsigned int FRAME_WIDTH, const unsigned int FRAME_HEIGHT, unsigned int curFrame)
	:	WIDTH_(FRAME_WIDTH), HEIGHT_(FRAME_HEIGHT)
{
	this->curFrame_ = curFrame_;
	this->numFrames_ = numFrames_;

	trueWidth_ = image.GetWidth();
	trueHeight_ = image.GetHeight();

	SetImage(image);

	Init();
}

SpriteSheet::SpriteSheet(unsigned int trueWidth, unsigned int trueHeight, unsigned int numFrames, const unsigned int FRAME_WIDTH, const unsigned int FRAME_HEIGHT, unsigned int curFrame)
	:	WIDTH_(FRAME_WIDTH), HEIGHT_(FRAME_HEIGHT)
{
	this->curFrame_ = curFrame;
	this->numFrames_ = numFrames;

	trueWidth_ = trueWidth;
	trueHeight_ = trueHeight;

	Init();
}

SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::Init()
{
	numColumns_ = trueWidth_ / WIDTH_;
	numRows_ = trueHeight_ / HEIGHT_;
	
	// Determines if the start of a new row is even or odd.
	((numColumns_ % 2) == 0) ? newRow_ = numColumns_ : newRow_ = (numColumns_+1);

	curColumn_ = curRow_ = 0;
	
	SetFrame(curFrame_);
}

sf::IntRect SpriteSheet::GetBounds()
{
	bounds_.Left = (int)GetPosition().x;
	bounds_.Top = (int)GetPosition().y;
	bounds_.Right = bounds_.Left + WIDTH_;
	bounds_.Bottom = bounds_.Top + HEIGHT_;

	return bounds_;	
}

void SpriteSheet::SetFrame(unsigned int frame)
{
	curFrame_ = frame;

	// Determines which frame of the sprite sheet should be displayed.
	if(curFrame_ != 0)
	{
		if((curFrame_ % newRow_) == 0) // On a new row.
		{
			curRow_ = int(floor((long double)curFrame_ / newRow_));
			curColumn_ = 0;
		}
		else // Progress through current row.
		{
			for(unsigned int i = 0; i < numColumns_; i++)
			{
				int rem = (i % numColumns_);
				if((curFrame_ % numColumns_) == rem)
				{
					curColumn_ = i;
					break;
				}
			}
		}
	}
	else
	{ 
		curColumn_ = curRow_ = 0;
	}

	const sf::IntRect subRect(curColumn_ * WIDTH_, curRow_ * HEIGHT_, (curColumn_ * WIDTH_) + WIDTH_, (curRow_ * HEIGHT_) + HEIGHT_);

	SetSubRect(subRect);
}
