#include "Button.h"

const sf::Vector2f Button::DEFAULT_VIEW(0.0f, 0.0f);

Button::Button(const unsigned int TRUE_BUTTON_WIDTH, const unsigned int TRUE_BUTTON_HEIGHT, const unsigned int BUTTON_WIDTH, const unsigned int BUTTON_HEIGHT) 
	:	SpriteSheet(TRUE_BUTTON_WIDTH, TRUE_BUTTON_HEIGHT, 5, BUTTON_WIDTH, BUTTON_HEIGHT)
{
	SetFrame(BUTTON_ACTIVE);

	active = true;
	prevState = BUTTON_ACTIVE;
	press = false;
}

Button::~Button()
{
}

void Button::Update(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view)
{
	if(active)
	{
		press = false;
		if(Over(INPUT, view))
		{
			if(Down(mouseButton, INPUT, view))
			{
				prevState = BUTTON_DOWN;
				SetFrame(BUTTON_DOWN);
			}
			else if(Up(mouseButton, INPUT, view))
			{
				prevState = BUTTON_UP;
				SetFrame(BUTTON_UP);
			}
			else
			{
				prevState = BUTTON_OVER;
				SetFrame(BUTTON_OVER);
			}
		}
		else
		{
			prevState = BUTTON_ACTIVE;
			SetFrame(BUTTON_ACTIVE);
		}
	}
	else
	{
		prevState = BUTTON_DISABLED;
		SetFrame(BUTTON_DISABLED);
	}
}

bool Button::Up(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view)
{
	if(Over(INPUT, view))
	{
		if(!Down(mouseButton, INPUT, view))
		{
			if(prevState == BUTTON_DOWN)
			{
				return true;
			}
		}
	}

	return false;
}

bool Button::Over(const sf::Input &INPUT, sf::Vector2f view)
{
	if(GetBounds().Contains(view.x + INPUT.GetMouseX(), view.y + INPUT.GetMouseY()))
	{
		return true;
	}

	return false;
}

bool Button::Down(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view)
{
	if(Over(INPUT, view))
	{
		if(INPUT.IsMouseButtonDown(mouseButton))
		{
			return true;
		}
	}

	return false;
}

bool Button::Pressed(sf::Mouse::Button mouseButton, const sf::Input &INPUT, sf::Vector2f view)
{
	if(Up(mouseButton, INPUT, view))
	{
		return true;
	}

	return false;
}

void Button::Enable()
{
	if(!active)
	{
		active = true;
		prevState = BUTTON_ACTIVE;
		SetFrame(BUTTON_ACTIVE);
	}
}

void Button::Disable()
{
	if(active)
	{
		active = false;
		prevState = BUTTON_DISABLED;
		SetFrame(BUTTON_DISABLED);
	}
}
