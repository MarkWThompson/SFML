#include "InputBox.h"

InputBox::InputBox(float x, float y, int boxWidth, int boxHeight, int inputWidth, int padding, float fontSize, const std::string FONT_FILEPATH, int maxChars, sf::Color colour)
{
	this->maxChars = maxChars;
	this->padding = padding;
	this->inputWidth = inputWidth;
	
	LoadFont(FONT_FILEPATH);

	curTextDisplay.SetFont(font);
	curTextDisplay.SetSize(fontSize); 
	curTextDisplay.SetColor(colour);
	curTextDisplay.SetPosition(x + padding, y + padding);
	
	backing.AddPoint(x, y); // Top left
	backing.AddPoint(x + boxWidth, y); // Top right
	backing.AddPoint(x + boxWidth, y + boxHeight); // Bottom right
	backing.AddPoint(x, y + boxHeight); // Bottom left

	backing.EnableFill(true);
	backing.EnableOutline(false);
	backing.SetColor(sf::Color::White);

	writingBound = int(inputWidth - padding);
}

InputBox::~InputBox()
{
}

std::string InputBox::GetInput()
{
	return entireBuffer;
}

void InputBox::SetText(std::string text)
{
	curTextDisplay.SetText(text);
	entireBuffer = visibleBuffer = text;

	Fit();
}

void InputBox::Fit()
{
	while(curTextDisplay.GetRect().Right - curTextDisplay.GetRect().Left > writingBound)
	{
		visibleBuffer.erase(visibleBuffer.begin());
		curTextDisplay.SetText(visibleBuffer);
	}
}

void InputBox::Update(sf::Event &events, bool eventFired, const sf::Input &input)
{
	if((events.Type == sf::Event::TextEntered) && (eventFired == true))
	{
		if ((events.Text.Unicode < 128) && (events.Text.Unicode != 8) && (events.Text.Unicode != 13) && (visibleBuffer.size() < (size_t)maxChars))//if the inputted text is in the unicode character set, and isnt delete, and isnt enter, and isnt larger than the limit
		{
			// The buffer that gets send over the network
			entireBuffer += static_cast<char>(events.Text.Unicode);

			// Add what we're typing to the visibleBuffer, then punch that over to the text object
			visibleBuffer += static_cast<char>(events.Text.Unicode);

			curTextDisplay.SetText(visibleBuffer);  // Sets the text	
			
			Fit();
		}
		else if (events.Text.Unicode == 8) // If the button pushed is backspace, deal with shortening the string and alligning it properly
		{
			if (entireBuffer.size () > 0)  // If there is something to remove from the end of the entireBuffer
			{
				entireBuffer.resize (entireBuffer.size () - 1); //remove the last char from the entireBuffer
			}
			if(visibleBuffer.size() > 0) // If the visibleBuffer is bigger than 0, ie if there is anything in the textfield
			{
				visibleBuffer.resize (visibleBuffer.size () - 1); // Remove the last char from the visibleBuffer

			}
			if(curTextDisplay.GetRect().Right < (writingBound)) // If the bottom chatframe isnt full
			{
				if(entireBuffer.size() - visibleBuffer.size() > 0) // If there is anything to scroll back too
				{
					visibleBuffer = entireBuffer[entireBuffer.size() - (visibleBuffer.size() + 1)] + visibleBuffer; // Set the visibleBuffer to the same but scrolled back one, using the entireBuffer as reference
				}
				curTextDisplay.SetText(visibleBuffer);
			}

			curTextDisplay.SetText(visibleBuffer); // Set the text once again
		}
	}
}

void InputBox::Draw(sf::RenderWindow* renderWindow)
{
	renderWindow->Draw(backing);
	renderWindow->Draw(curTextDisplay);
}

sf::Vector2f InputBox::GetPosition()
{
	return backing.GetPosition();
}

void InputBox::SetPosition(float x, float y)
{
	backing.SetPosition(x, y);
	curTextDisplay.SetPosition(x + padding, y + padding);
	writingBound = int(backing.GetPosition().x + inputWidth - curTextDisplay.GetSize() - padding);
}

void InputBox::EraseBuffers()
{
	visibleBuffer.clear();
	entireBuffer.clear();
	curTextDisplay.SetText(entireBuffer);
}

bool InputBox::LoadFont(const std::string FONT_FILEPATH)
{
	if(!font.LoadFromFile(FONT_FILEPATH))
	{
		std::cout << "InputBox::LoadFont() error : Could not load font from specified filepath." << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}
