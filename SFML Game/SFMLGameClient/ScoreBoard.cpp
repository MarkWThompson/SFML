#include "ScoreBoard.h"

//prototype
//The pair comparatator, uses the second var
bool sortPairBySecond (const std::pair<sf::String, int>& i, const std::pair<sf::String, int>& j);

ScoreBoard::ScoreBoard()
{
	if(!frameImage.LoadFromFile(HIGHSCOREFRAME_IMAGE))
	{
		std::cout << "Failed to load ScoreFrameImage" << std::endl;
	}
	frameSprite.SetImage(frameImage);

	playerNamesStartOffset.x = 20;
	playerNamesStartOffset.y = 21;

	playerNamesHorizontalOffset = 38;

	horizontalScoreOffset = 168;

	shouldShow = true;
	positionsSet = false;
}

ScoreBoard::ScoreBoard(int x, int y)
{
	if(!frameImage.LoadFromFile(HIGHSCOREFRAME_IMAGE))
	{
		std::cout << "Failed to load ScoreFrameImage" << std::endl;
	}
	frameSprite.SetImage(frameImage);

	playerNamesStartOffset.x = 20;
	playerNamesStartOffset.y = 21;

	playerNamesHorizontalOffset = 38;

	horizontalScoreOffset = 168;

	shouldShow = true;

	scoreBoardPosition.x = x;
	scoreBoardPosition.y = y;
	frameSprite.SetPosition(scoreBoardPosition);
	positionsSet = false;
}


ScoreBoard::~ScoreBoard()
{
}

void ScoreBoard::Update()
{
}

void ScoreBoard::UpdateScores(std::vector<sf::Int16> inputPlayerScores, std::vector<sf::String> playerNames)
{

	for(size_t i = 0; i < playerNames.size(); i++)
	{
		if(playerStrings.size() <= i)
		{
			std::pair<sf::String, int> newPlayerPair;
			sf::String newPlayerString;
			newPlayerString.SetText(playerNames[i].GetText());
			newPlayerString.SetSize(15);
			newPlayerPair.first = newPlayerString;
			newPlayerPair.second = inputPlayerScores[i];
			newPlayerPair.first.SetX(scoreBoardPosition.x + playerNamesStartOffset.x);
			newPlayerPair.first.SetY((scoreBoardPosition.y + playerNamesStartOffset.y) + (playerNamesHorizontalOffset * i));
			playerStrings.push_back(newPlayerPair);

			// Checks for no name, puts in a placeholder
			std::string cowardCheck;
			cowardCheck = playerNames[i].GetText();
			if(cowardCheck.size() == 0)
			{
				playerStrings[i].first.SetText("Nameless Coward");
			}
		}
		else
		{
			playerStrings[i].first.SetText(playerNames[i].GetText());
			playerStrings[i].second = inputPlayerScores[i];

			// Checks for no name, puts in a placeholder
			std::string cowardCheck;
			cowardCheck = playerNames[i].GetText();
			if(cowardCheck.size() == 0)
			{
				playerStrings[i].first.SetText("Nameless Coward");
			}
		}
	}

	// Get the scores
	for(size_t i = 0; i < inputPlayerScores.size(); i++)
	{
		// Convert this score to a string
		std::stringstream ss;
		ss << inputPlayerScores[i];

		std::pair<sf::String,int> playerScoreString;
		playerScoreString.first.SetText(ss.str());
		playerScoreString.first.SetSize(15);
		playerScoreString.second = inputPlayerScores[i];
		playerScoreString.first.SetX(scoreBoardPosition.x + playerNamesStartOffset.x + horizontalScoreOffset);
		playerScoreString.first.SetY((scoreBoardPosition.y + playerNamesStartOffset.y) + (playerNamesHorizontalOffset * i));

		if(scoreStrings.size() <= i)
		{
			playerScores.push_back(inputPlayerScores[i]);
			scoreStrings.push_back(playerScoreString);
		}
		else
		{
			playerScores[i] = inputPlayerScores[i];
			scoreStrings[i] = playerScoreString;
		}
	}

	// Sort the scores
	std::sort(playerStrings.begin(),playerStrings.end(),sortPairBySecond);
	std::sort(scoreStrings.begin(), scoreStrings.end(),sortPairBySecond);

	/*
		// Set the player name positions
		for(size_t i = 0; i <playerStrings.size(); i++)
		{
			playerStrings[i].first.SetX(scoreBoardPosition.x + playerNamesStartOffset.x);
			playerStrings[i].first.SetY((scoreBoardPosition.y + playerNamesStartOffset.y) + (playerNamesHorizontalOffset * i));
			scoreStrings[i].first.SetX(scoreBoardPosition.x + playerNamesStartOffset.x + horizontalScoreOffset);
			scoreStrings[i].first.SetY((scoreBoardPosition.y + playerNamesStartOffset.y) + (playerNamesHorizontalOffset * i));
		}
		*/
}


void ScoreBoard::SetPosition(float x, float y)
{
	scoreBoardPosition.x = x;
	scoreBoardPosition.y = y;
	frameSprite.SetPosition(scoreBoardPosition);

	// Set the player name positions
	for(size_t i = 0; i <playerStrings.size(); i++)
	{
		playerStrings[i].first.SetX(scoreBoardPosition.x + playerNamesStartOffset.x);
		playerStrings[i].first.SetY((scoreBoardPosition.y + playerNamesStartOffset.y) + (playerNamesHorizontalOffset * i));
		scoreStrings[i].first.SetX(scoreBoardPosition.x + playerNamesStartOffset.x + horizontalScoreOffset);
		scoreStrings[i].first.SetY((scoreBoardPosition.y + playerNamesStartOffset.y) + (playerNamesHorizontalOffset * i));
	}
}
void ScoreBoard::Render(sf::RenderWindow &renderWindow)
{
	renderWindow.Draw(frameSprite);
	
	// Draw the playerStrings
	for(size_t i = 0; i < playerStrings.size(); i++)
	{
		renderWindow.Draw(playerStrings[i].first);
	}

	// Draw the scoreStrings
	for(size_t i = 0; i < scoreStrings.size(); i++)
	{
		renderWindow.Draw(scoreStrings[i].first);
	}

}

sf::Sprite &ScoreBoard::GetFrameSprite()
{
	return frameSprite;
}

bool ScoreBoard::ShouldShow()
{
	return shouldShow;
}

void ScoreBoard::SetShouldShow(bool shouldShow)
{
	this->shouldShow = shouldShow;
}

// Used as a comparator for sorting a pair by its second element
bool sortPairBySecond (const std::pair<sf::String, int>& i, const std::pair<sf::String, int>& j)
{ 
	return (i.second > j.second); 
}

