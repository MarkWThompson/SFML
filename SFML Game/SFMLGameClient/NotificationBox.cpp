#include "NotificationBox.h"

NotificationBox::NotificationBox()
	:	MAX_CHARS(512), WIDTH(400.0f), DEFAULT_HEIGHT(65.0f), okButton(600, 40, 120, 40),
		PADDING(30.0f), MAX_STRING_WIDTH(350.0f)
{
	workingID = curID = -1;

	backing.EnableFill(true);
	backing.EnableOutline(false);
	backing.SetColor(sf::Color(204, 204, 204)); // Light grey
	backing.AddPoint(0.0f, 0.0f); // Top left
	backing.AddPoint(WIDTH, 0.0f); // Top right
	backing.AddPoint(WIDTH, DEFAULT_HEIGHT); // Bottom right
	backing.AddPoint(0.0f, DEFAULT_HEIGHT); // Bottom left

	backingWidth = WIDTH;
	backingHeight = 0.0f;

	moireFont.LoadFromFile(MOIRE_FONT_FILE);

	displayText.SetColor(sf::Color::Black);
	displayText.SetFont(moireFont);
	displayText.SetSize(20.0f);

	okButtonImage.LoadFromFile(OK_BUTTON_IMAGE);
	okButton.SetImage(okButtonImage);
}

void NotificationBox::FormatDisplayText()
{
	// Make sure the string fits nicely into the backing box
	if(displayText.GetRect().GetWidth() > MAX_STRING_WIDTH)
	{
		std::string str = displayText.GetText();
		std::vector<std::string> wordBuffer;
		std::string word;

		for(size_t i = 0; i < str.length(); i++)
		{
			if(isspace(str.at(i)) || (i == str.length() - 1))
			{
				if(word.length() > MAX_STRING_WIDTH)
				{
					std::cout << "NotificationBox::NotificationBox() error : word lenth > MAX_STRING_WIDTH." << std::endl;
					break;
				}
				else
				{
					wordBuffer.push_back(word);
					word = "";
				}
			}
			else
			{
				word += str.at(i);
			}
		}

		std::string newStr;
		float lineSize = 0.0f;

		for(size_t i = 0; i < wordBuffer.size(); i++)
		{
			displayText.SetText(wordBuffer[i]);
			float strWidth = displayText.GetRect().GetWidth();

			if(strWidth + lineSize < MAX_STRING_WIDTH)
			{
				newStr += (wordBuffer[i] + ' ');
				lineSize += strWidth;
			}
			else
			{
				newStr += ('\n' + wordBuffer[i] + ' ');
				lineSize = strWidth;
			}
		}

		displayText.SetText(newStr);
	}
}

void NotificationBox::CalculateBackingDimensions(bool hasButton)
{
	backing.SetPointPosition(0, 0.0f, 0.0f); // Top left
	backing.SetPointPosition(1, WIDTH, 0.0f); // Top right

	if(hasButton)
	{
		backing.SetPointPosition(2, WIDTH, DEFAULT_HEIGHT + displayText.GetRect().GetHeight() + (PADDING * 2)); // Bottom right
		backing.SetPointPosition(3, 0.0f, DEFAULT_HEIGHT + displayText.GetRect().GetHeight() + (PADDING * 2)); // Bottom left
		backingHeight = DEFAULT_HEIGHT + displayText.GetRect().GetHeight() + (PADDING * 2);
	}
	else
	{
		backing.SetPointPosition(2, WIDTH, displayText.GetRect().GetHeight() + (PADDING * 2)); // Bottom right
		backing.SetPointPosition(3, 0.0f, displayText.GetRect().GetHeight() + (PADDING * 2)); // Bottom left
		backingHeight = displayText.GetRect().GetHeight() + (PADDING * 2);
	}
}

void NotificationBox::Reposition()
{
	backing.SetPosition((1280.0f / 2.0f) - (backingWidth / 2.0f), (720.0f / 2.0f) - (backingHeight / 2.0f));
	displayText.SetPosition(backing.GetPosition().x + PADDING, backing.GetPosition().y + PADDING);
	okButton.SetPosition((1280.0f / 2.0f) - (120.0f / 2.0f), backing.GetPosition().y + displayText.GetRect().GetHeight() + (PADDING * 2));
}

NotificationBox::~NotificationBox()
{
}

NotificationBox& NotificationBox::GetInstance()
{
	static NotificationBox instance;
	return instance;
}

int NotificationBox::AddNotification(std::string message, bool hasButton)
{
	if(message.length() <= MAX_CHARS)
	{
		curID++;
		Notification notification(message, hasButton, curID);
		

		notificationBuffer.push_back(notification);

		//std::cout << "Notification added." << std::endl;
	}
	else
	{
		std::cout << "NotificationBox::AddNotification() error : message parameter has too many chars." << std::endl;
	}
	
	// Value given to notification
	return curID;
}

void NotificationBox::Update(const sf::Input& INPUT)
{
	if(!notificationBuffer.empty())
	{
		Notification notification = notificationBuffer.front();

		// Update notification if different from previous notification
		if(workingID != notification.id)
		{
			workingID = notification.id;
			displayText.SetText(notification.message);

			FormatDisplayText();
			CalculateBackingDimensions(notification.hasButton);
			Reposition();
		}

		// Handle button press
		if(notification.hasButton)
		{
			if(okButton.Pressed(sf::Mouse::Left, INPUT))
			{
				RemoveNotification(notification.id);
			}

			okButton.Update(sf::Mouse::Left, INPUT);
		}
	}
}

void NotificationBox::Render(sf::RenderWindow* renderWindow)
{
	if(!notificationBuffer.empty())
	{
		renderWindow->Draw(backing);
		renderWindow->Draw(displayText);
		
		if(notificationBuffer.front().hasButton)
		{
			renderWindow->Draw(okButton);
		}
	}
}

bool NotificationBox::RemoveNotification(int id)
{
	if(!notificationBuffer.empty())
	{
		for(size_t i = 0; i < notificationBuffer.size(); i++)
		{
			if(notificationBuffer[i].id == id)
			{
				notificationBuffer.erase(notificationBuffer.begin() + i);
				return true;
			}
		}
	}

	return false;
}
