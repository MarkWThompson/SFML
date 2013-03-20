#include "ScoreBoard.h"

//prototype
//The pair comparatator, uses the second var
bool sortPairBySecond (const std::pair<sf::String, int>& i, const std::pair<sf::String, int>& j);


ScoreBoard::ScoreBoard()
{
	frameImage.LoadFromFile(HIGHSCOREFRAME_IMAGE);
	frameSprite.SetImage(frameImage);
	frameSprite.SetPosition(scoreBoardPosition);

	playerNamesStartOffset.x = 40;
	playerNamesStartOffset.y = 42;

	playerNamesHorizontalOffset = 76;

	horizontalScoreOffset = 335;

}


ScoreBoard::~ScoreBoard(void)
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
			newPlayerString.SetSize(30);
			newPlayerPair.first = newPlayerString;
			newPlayerPair.second = inputPlayerScores[i];
			playerStrings.push_back(newPlayerPair);

			//Checks for no name, puts in a placeholder
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

			//Checks for no name, puts in a placeholder
			std::string cowardCheck;
			cowardCheck = playerNames[i].GetText();
			if(cowardCheck.size() == 0)
			{
				playerStrings[i].first.SetText("Nameless Coward");
			}
		}
	}

	//Get the scores
	for(size_t i = 0; i < inputPlayerScores.size(); i++)
	{
		//convert this score to a string
		std::stringstream ss;
		ss << inputPlayerScores[i];

		std::pair<sf::String,int> playerScoreString;
		playerScoreString.first.SetText(ss.str());
		playerScoreString.second = inputPlayerScores[i];

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

	//Sort the scores
	std::sort(playerStrings.begin(),playerStrings.end(),sortPairBySecond);
	std::sort(scoreStrings.begin(), scoreStrings.end(),sortPairBySecond);
}

void ScoreBoard::CalculatePosition(sf::FloatRect viewRect)
{
	scoreBoardPosition.x = ((viewRect.Left + viewRect.Right) /2) -  (frameSprite.GetSize().x/2);
	scoreBoardPosition.y = ((viewRect.Top + viewRect.Bottom) /2) -  (frameSprite.GetSize().y/2);
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
	
	//Draw the playerStrings
	for(size_t i = 0; i < playerStrings.size(); i++)
	{
		renderWindow.Draw(playerStrings[i].first);
	}

	//draw the scoreStrings
	for(size_t i = 0; i < scoreStrings.size(); i++)
	{
		renderWindow.Draw(scoreStrings[i].first);
	}

}

//Used as a comparator for sorting a pair by its second element
bool sortPairBySecond (const std::pair<sf::String, int>& i, const std::pair<sf::String, int>& j)
{ 
	return (i.second > j.second); 
}