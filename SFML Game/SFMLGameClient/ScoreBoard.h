#pragma once

#include <SFML\Graphics.hpp>
#include "Assets.h"
#include <iostream>
#include <sstream>

class ScoreBoard
{
public:
	ScoreBoard();
	ScoreBoard(int x, int y);

	~ScoreBoard();

	void Update();
	void Render(sf::RenderWindow &renderWindow);
	void UpdateScores(std::vector<sf::Int16> inputPlayerScores, std::vector<sf::String> playerNames);
	void SetPosition(float x, float y);

	bool ShouldShow();
	void SetShouldShow(bool shouldShow);

	sf::Sprite &GetFrameSprite();

private:
	sf::Image frameImage;
	sf::Sprite frameSprite;
	std::vector<std::pair<sf::String, int>> playerStrings;
	std::vector<std::pair<sf::String, int>> scoreStrings;
	std::vector<int> playerScores;
	sf::Vector2f scoreBoardPosition;

	sf::Vector2f playerNamesStartOffset;
	float playerNamesHorizontalOffset;
	float horizontalScoreOffset;

	bool positionsSet;
	bool shouldShow;
};

