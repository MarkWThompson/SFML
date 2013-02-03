#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "TextInputBox.h"
#include <iostream>

class StartScreen
{
public:
	StartScreen();
	StartScreen(sf::RenderWindow &renderWindow, std::string ipFontPath, float fontSize, int boxWidth);
	~StartScreen();

	void Init();
	void Update(sf::Event events,const sf::Input &input);
	void Draw(sf::RenderWindow &renderWindow);
	sf::IPAddress GetAndEraseIP();

private:
	sf::Image startScreenImage;
	sf::Sprite startScreen;
	TextInputBox* ipInput;
	bool isConnecting;

};

