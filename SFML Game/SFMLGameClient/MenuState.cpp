#include "MenuState.h"

MenuState::MenuState(SharedConstants::StateID ID)
	:	State(ID),
		lobbyButton(860, 72, 172, 72),
		optionsButton(860, 72, 172, 72),
		helpButton(860, 72, 172, 72),
		creditsButton(860, 72, 172, 72)
{
}

bool MenuState::Load()
{
	if(backgroundImage.LoadFromFile(MENU_BG_IMAGE))
	{
		backgroundSprite.SetImage(backgroundImage);
		backgroundSprite.SetPosition(0.0f, 0.0f);
	}
	else
	{
		return false;
	}

	if(lobbyButtonImage.LoadFromFile(LOBBY_BUTTON_IMAGE))
	{
		lobbyButton.SetImage(lobbyButtonImage);
		lobbyButton.SetPosition(240.0f, 620.0f);
	}
	else
	{
		return false;
	}

	if(optionsButtonImage.LoadFromFile(OPTIONS_BUTTON_IMAGE))
	{
		optionsButton.SetImage(optionsButtonImage);
		optionsButton.SetPosition(450.0f, 620.0f);
	}
	else
	{
		return false;
	}

	if(helpButtonImage.LoadFromFile(HELP_BUTTON_IMAGE))
	{
		helpButton.SetImage(helpButtonImage);
		helpButton.SetPosition(660.0f, 620.0f);
	}
	else
	{
		return false;
	}

	if(creditsButtonImage.LoadFromFile(CREDITS_BUTTON_IMAGE))
	{
		creditsButton.SetImage(creditsButtonImage);
		creditsButton.SetPosition(870.0f, 620.0f);
	}
	else
	{
		return false;
	}

	if(MenuLoad())
	{
		return true;
	}
}

MenuState::~MenuState()
{

}

void MenuState::Update(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
	// Button press
	if(lobbyButton.Pressed(sf::Mouse::Left, INPUT))
	{
		SwitchState(SharedConstants::LOBBY_STATE);
		audioManager.mousePressedSFX.Play();
	}
	
	if(optionsButton.Pressed(sf::Mouse::Left, INPUT))
	{
		SwitchState(SharedConstants::OPTIONS_STATE);
		audioManager.mousePressedSFX.Play();
	}

	if(helpButton.Pressed(sf::Mouse::Left, INPUT))
	{
		SwitchState(SharedConstants::HELP_STATE);
		audioManager.mousePressedSFX.Play();
	}

	if(creditsButton.Pressed(sf::Mouse::Left, INPUT))
	{
		SwitchState(SharedConstants::CREDITS_STATE);
		audioManager.mousePressedSFX.Play();
	}

	// Update all buttons
	lobbyButton.Update(sf::Mouse::Left, INPUT);
	optionsButton.Update(sf::Mouse::Left, INPUT);
	helpButton.Update(sf::Mouse::Left, INPUT);
	creditsButton.Update(sf::Mouse::Left, INPUT);

	starfield.Animate();

	MenuUpdate(events, eventFired, INPUT);
}

void MenuState::Draw(sf::RenderWindow* renderWindow)
{
	renderWindow->Draw(backgroundSprite);
	starfield.Render(renderWindow);
	renderWindow->Draw(lobbyButton);
	renderWindow->Draw(optionsButton);
	renderWindow->Draw(helpButton);
	renderWindow->Draw(creditsButton);
	renderWindow->Draw(overlaySprite);
	
	MenuDraw(renderWindow);
}
