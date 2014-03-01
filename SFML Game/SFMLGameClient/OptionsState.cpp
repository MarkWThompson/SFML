#include "OptionsState.h"
#include "SimplePersistence.h"

OptionsState::OptionsState()
	: MenuState(SharedConstants::OPTIONS_STATE),
	generalButton(1100, 40, 220, 40),
	displayButton(1100, 40, 220, 40),
	audioButton(1100, 40, 220, 40),
	changeButton(600, 40, 120, 40),
	MAX_MESSAGE_TIME(3.0f), USERNAME_MAX_CHARS(10)
{
	displayButton.Disable();
	audioButton.Disable();

	curOption = GENERAL;

	usernameInputBox = new InputBox(690.0f, 285.0f, 470, 60, 470, 10, 30.0f, MOIRE_FONT_FILE);

	// Grab the username
	if(sPersistence.HasEntry("Username"))
	{
		usernameInputBox->SetText(sPersistence.GetString("Username"));
	}
}

bool OptionsState::MenuLoad()
{
	if(overlayImage.LoadFromFile(OPTIONS_OVERLAY_IMAGE))
	{
		overlaySprite.SetImage(overlayImage);
	}
	else
	{
		return false;
	}

	if(changeButtonImage.LoadFromFile(CHANGE_BUTTON_IMAGE))
	{
		changeButton.SetImage(changeButtonImage);
		changeButton.SetPosition(690.0f, 355.0f);
	}
	else
	{
		return false;
	}

	if(generalButtonImage.LoadFromFile(GENERAL_BUTTON_IMAGE))
	{
		generalButton.SetImage(generalButtonImage);
		generalButton.SetPosition(120.0f, 250.0f);
	}
	else
	{
		return false;
	}

	if(audioButtonImage.LoadFromFile(AUDIO_BUTTON_IMAGE))
	{
		audioButton.SetImage(audioButtonImage);
		audioButton.SetPosition(120.0f, 300.0f);
	}
	else
	{
		return false;
	}

	if(displayButtonImage.LoadFromFile(DISPLAY_BUTTON_IMAGE))
	{
		displayButton.SetImage(displayButtonImage);
		displayButton.SetPosition(120.0f, 350.0f);
	}
	else
	{
		return false;
	}

	if(font.LoadFromFile(MOIRE_FONT_FILE))
	{
		message.SetFont(font);
		message.SetSize(15); 
		message.SetColor(sf::Color::White);
		message.SetPosition(830.0f, 365.0f);
	}
	else
	{
		std::cout << "OptionsState::MenuLoad() error : Could not load font from specified filepath." << std::endl;
		return false;
	}

	

	return true;
}

OptionsState::~OptionsState()
{
}

void OptionsState::MenuUpdate(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
	if(generalButton.Pressed(sf::Mouse::Left, INPUT))
	{
		curOption = GENERAL;
	}

	if(audioButton.Pressed(sf::Mouse::Left, INPUT))
	{
		curOption = AUDIO;
	}

	if(displayButton.Pressed(sf::Mouse::Left, INPUT))
	{
		curOption = DISPLAY;
	}

	// Update buttons
	audioButton.Update(sf::Mouse::Left, INPUT);
	generalButton.Update(sf::Mouse::Left, INPUT);
	displayButton.Update(sf::Mouse::Left, INPUT);

	switch(curOption)
	{
		case GENERAL:
			HandleGeneralOption(events, eventFired, INPUT);
			break;

		case AUDIO:
			HandleAudioOption();
			break;

		case DISPLAY:
			HandleDisplayOption();
			break;
	}
}

void OptionsState::MenuDraw(sf::RenderWindow* renderWindow)
{
	renderWindow->Draw(audioButton);
	renderWindow->Draw(generalButton);
	renderWindow->Draw(displayButton);

	switch(curOption)
	{
		case GENERAL:
			renderWindow->Draw(message);
			usernameInputBox->Draw(renderWindow);
			renderWindow->Draw(changeButton);
			break;

		case AUDIO:
			break;

		case DISPLAY:
			break;
	}
}

void OptionsState::SetView(sf::RenderWindow* renderWindow)
{
}

void OptionsState::ReceiveData(sf::Packet receivedPacket, sf::IPAddress connectionAddress, unsigned int port)
{
}

void OptionsState::HandleGeneralOption(sf::Event events, bool eventFired, const sf::Input &INPUT)
{
	usernameInputBox->Update(events, eventFired, INPUT);

	// Error handling
	if(usernameInputBox->GetInput().empty())
	{
		changeButton.Disable();
		message.SetText("Username cannot be empty");
		message.SetColor(sf::Color::Red);
	}
	else if(usernameInputBox->GetInput().length() > USERNAME_MAX_CHARS)
	{
		changeButton.Disable();
		message.SetText("Username too long (10 character limit)");
		message.SetColor(sf::Color::Red);
	}
	else
	{
		changeButton.Enable();

		// Allow success message to show before hiding message
		if(messageTime.GetElapsedTime() > MAX_MESSAGE_TIME)
		{
			message.SetText("");
		}
	}

	if(changeButton.Pressed(sf::Mouse::Left, INPUT))
	{
		sPersistence.SetString("Username", usernameInputBox->GetInput());
		sPersistence.Save();
		message.SetText("Username successfully changed!");
		message.SetColor(sf::Color::Green);
		messageTime.Reset();
	}

	changeButton.Update(sf::Mouse::Left, INPUT);
}

void OptionsState::HandleAudioOption()
{
}

void OptionsState::HandleDisplayOption()
{
}
