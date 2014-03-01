#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "Assets.h"
#include "Button.h"
#include <iostream>
#include <vector>

// Macro for quickly accessing the singleton instance
#define notificationBox NotificationBox::GetInstance()

class NotificationBox
{
public:
	/** Returns the singleton instance of the class. */
	static NotificationBox& GetInstance();

	/** 
		Adds a notification to the buffer.
		@return Returns unique ID which can be used to remove the notification.
	*/
	int AddNotification(std::string message, bool enableButton);

	void Update(const sf::Input& INPUT);

	/** Renders any pending notifications. */
	void Render(sf::RenderWindow* renderWindow);

	/**
		Removes a notification with unique id from the buffer.
		@return Returns true if the specified notification was removed, otherwise returns false.
	*/
	bool RemoveNotification(int id);

private:
	void Reposition();
	void FormatDisplayText();
	void CalculateBackingDimensions(bool hasButton);

	int workingID;
	int curID;

	struct Notification
	{
		std::string message;
		bool hasButton;
		int id;

		Notification(std::string message, bool hasButton, int id)
		{
			this->message = message;
			this->hasButton = hasButton;
			this->id = id;
		}

		// Could add timing facility for timed messages
	};

	NotificationBox();
	~NotificationBox();

	float backingWidth;
	float backingHeight;

	/** The maximum number of characters of a notification. */
	const int MAX_CHARS;

	/** The width of the notification box. */
	const float WIDTH;

	const float DEFAULT_HEIGHT;

	const float PADDING;

	const float MAX_STRING_WIDTH;

	/** Rectangular backing representing the box. */
	sf::Shape backing;

	/** The text which is currently being displayed. */
	sf::String displayText;

	/** A buffer of notifications. */
	std::vector<Notification> notificationBuffer;

	
	sf::Image okButtonImage;
	Button okButton;

	sf::Font moireFont;

	/* 
		Should not be implemented in MessageBox.cpp to ensure that no 
		more than one instance of the MessageBox class is created. 
	*/
    NotificationBox(NotificationBox const&);
    void operator=(NotificationBox const&);
};
