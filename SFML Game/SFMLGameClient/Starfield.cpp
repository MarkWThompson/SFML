#include "Starfield.h"

Starfield::Starfield()
	:	PERSPECTIVE_PLANE_DIST(500.0f), STARFIELD_SPEED(1),
		Z_SCALING_FACTOR(1.0f), NUM_STARS(50),
		MAX_DEPTH(500)
{
	for(int i = 0; i < NUM_STARS; i++)
	{
		starfield.push_back(CreateStar());

		sf::Vector3i point = sf::Vector3i(sf::Randomizer::Random(-640, 640), sf::Randomizer::Random(-360, 360), sf::Randomizer::Random(-1200, 400));
		starPositions.push_back(point);
	}
}

Starfield::~Starfield()
{
}

void Starfield::Animate()
{
	for(size_t i = 0; i < starfield.size(); i++)
	{
		float perspective = PERSPECTIVE_PLANE_DIST / (PERSPECTIVE_PLANE_DIST - (float)starPositions[i].z);
		
		float newX = (float)starPositions[i].x * perspective;
		float newY = (float)starPositions[i].y * perspective;
		float scale = (Z_SCALING_FACTOR * perspective);

		starfield[i].SetPosition(640.0f + newX, 360.0f + newY);
		starfield[i].SetScale(scale, scale);

		starPositions[i].z += STARFIELD_SPEED;

		if(starPositions[i].z > MAX_DEPTH)
		{
			starPositions[i].x = sf::Randomizer::Random(-640, 640);
			starPositions[i].y = sf::Randomizer::Random(-360, 360);
			starPositions[i].z -= 1200;
		}
	}
}

sf::Shape Starfield::CreateStar()
{
	sf::Uint8 alpha = (sf::Uint8)sf::Randomizer::Random(50, 255);
	sf::Color colour = sf::Color(255, 255, 255, alpha);
	float radius = 1.0f;

	// Centre screen
	sf::Vector2f centre = sf::Vector2f(0.0f, 0.0f);

	sf::Shape star = sf::Shape::Circle(centre, radius, colour);
	
	return star;
}

void Starfield::Render(sf::RenderWindow* renderWindow)
{
	for(int i = 0; i < NUM_STARS; i++)
	{
		renderWindow->Draw(starfield[i]);
	}
}
