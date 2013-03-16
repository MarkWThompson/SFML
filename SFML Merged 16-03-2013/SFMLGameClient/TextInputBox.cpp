#include "TextInputBox.h"

TextInputBox::TextInputBox()
{
	maxChars = 255;
	
	LoadFont("Impact.ttf");

	currentTextDisplay.SetFont(font);
	currentTextDisplay.SetSize(12.0f); 
	currentTextDisplay.SetColor(sf::Color(0, 0, 0));
	currentTextDisplay.SetPosition(0, 0);

	rightTextBound = int(currentTextDisplay.GetPosition().x + 100);

	bounds.Left = 0;
	bounds.Right = 100;
	bounds.Top = 0;
	bounds.Bottom = 12;

	shouldEnter = true;
}

TextInputBox::TextInputBox(float x, float y, int boxWidth, float fontSize, std::string fontPath, int maxChars, sf::Color colour)
{
	this->maxChars = maxChars;
	
	LoadFont(fontPath);

	currentTextDisplay.SetFont(font);
	currentTextDisplay.SetSize(fontSize); 
	currentTextDisplay.SetColor(colour);
	currentTextDisplay.SetPosition(x, y);

	bounds.Left = x - 10;
	bounds.Right = x + boxWidth + 50;
	bounds.Top = y - 10;
	bounds.Bottom = y + fontSize + 10;

	rightTextBound = int(currentTextDisplay.GetPosition().x + boxWidth);

	shouldEnter = true;
}

TextInputBox::~TextInputBox()
{
}

void TextInputBox::Update(sf::Event &events, bool eventFired, const sf::Input &input)
{
	HandleLocalTextInput(events,eventFired,input);
}

void TextInputBox::HandleLocalTextInput(sf::Event &events, bool eventFired, const sf::Input &input)
{

	if((events.Type == sf::Event::TextEntered) && (eventFired == true))
	{
		if ((events.Text.Unicode < 128) && (events.Text.Unicode != 8) && (events.Text.Unicode != 13) && (tempBuffer.size() < maxChars))//if the inputted text is in the unicode character set, and isnt delete, and isnt enter, and isnt larger than the limit
		{
			//In general the sendBuffer has the entire message, and the tempBuffer has what is being displayed in the typing field. The currentTextDisplay is the actual text object that displays it
			//The buffer that gets send over the network
			sendBuffer += static_cast<char>(events.Text.Unicode);

			//add what we're typing to the tempbuffer, then punch that over to the text object
			tempBuffer += static_cast<char>(events.Text.Unicode);

			currentTextDisplay.SetText(tempBuffer);  //sets the text	
			
			if(currentTextDisplay.GetRect().Right > rightTextBound) //Gets the position of the final letter of the displayed text(by using the tempBuffer as reference) Then checks if it is too far along
			{
				int charsToShiftOff = int((currentTextDisplay.GetRect().Right - (rightTextBound)) / currentTextDisplay.GetSize()); //some letters are bigger than the others, so this calculates the number of letters (normally just 1 or 2,) to shift off the left
				
				for(int i = 0; i < charsToShiftOff; i++)
				{
					tempBuffer.erase(tempBuffer.begin());	//This simple loop erases any letters that need to be erased
				}
				currentTextDisplay.SetText(tempBuffer); //sets the text again
			}
		}
		else if (events.Text.Unicode == 8) //If the button pushed is delete, deal with shortening the string and alligning it properly
		{
			if (sendBuffer.size () > 0)  //if there is somethign to remove from the end of the sendBuffer
			{
				sendBuffer.resize (sendBuffer.size () - 1); //remove the last char from the sendBuffer
			}
			if(tempBuffer.size() > 0) //if the tempBuffer is bigger than 0, ie if there is anything in the textfield
			{
				tempBuffer.resize (tempBuffer.size () - 1); //remove the last char from the tempBuffer

			}
			if(currentTextDisplay.GetRect().Right < (rightTextBound)) // if the bottom chatframe isnt full
			{
				if(sendBuffer.size() - tempBuffer.size() > 0) //if there is anything to scroll back too
				{
					tempBuffer = sendBuffer[sendBuffer.size() - (tempBuffer.size() + 1)] + tempBuffer; //set the tempBuffer to the same but scrolled back one, using the sendBuffer as reference
				}
				currentTextDisplay.SetText(tempBuffer);
			}
			currentTextDisplay.SetText(tempBuffer); //set the text once again
		}
	}
}
void TextInputBox::Draw(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(currentTextDisplay);
}

void TextInputBox::LoadFont(std::string fontPath)
{
	if(!font.LoadFromFile(fontPath))
	{
		std::cout << "TextInputBox font was not loaded." << std::endl;
	}
}

sf::Vector2f TextInputBox::GetPosition()
{
	return currentTextDisplay.GetPosition();
}

void TextInputBox::SetPosition(float x, float y)
{
	currentTextDisplay.SetPosition(x, y);
}

void TextInputBox::EraseBuffers()
{
	tempBuffer.clear();
	sendBuffer.clear();
	currentTextDisplay.SetText(sendBuffer);
}
