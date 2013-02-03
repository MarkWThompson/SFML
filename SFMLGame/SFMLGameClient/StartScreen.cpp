#include "StartScreen.h"

StartScreen::StartScreen()
{
	ipInput = new TextInputBox(260, 240, 700, 50, "Impact.ttf");

	Init();
}

StartScreen::StartScreen(sf::RenderWindow &renderWindow, std::string ipFontPath, float fontSize, int boxWidth)
{
	ipInput = new TextInputBox(260.0f, 240.0f, boxWidth, fontSize, ipFontPath);

	Init();
}

void StartScreen::Init()
{
	// Load in the image
	if(startScreenImage.LoadFromFile("StartScreen.png"))
	{
		startScreen.SetImage(startScreenImage);
	}

	startScreen.SetPosition(0.0f, 0.0f);
	isConnecting = false;
}

StartScreen::~StartScreen()
{
}

void StartScreen::Update(sf::Event events, const sf::Input &input)
{
	ipInput->Update(events);
	if((input.IsKeyDown(sf::Key::Return)) && (isConnecting == false))
	{
		isConnecting = true;
	}
	if(input.IsKeyDown(sf::Key::Return) == false)
	{
		isConnecting = false;
	}
}

void StartScreen::Draw(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(startScreen);
	ipInput->Draw(renderWindow);
}

sf::IPAddress StartScreen::GetAndEraseIP()
{
	sf::IPAddress returnAddress = ipInput->sendBuffer;
	ipInput->EraseBuffers();

	return returnAddress;
}
