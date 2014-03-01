#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include <iostream>

class Starfield
{
public:
	Starfield();
	~Starfield();

	void Animate();
	void Render(sf::RenderWindow* renderWindow);

private:

	sf::Shape CreateStar();

	std::vector<sf::Shape> starfield;
	std::vector<sf::Vector3i> starPositions;
	const float PERSPECTIVE_PLANE_DIST;
	const int STARFIELD_SPEED;
	const float Z_SCALING_FACTOR;
	const int NUM_STARS;
	const int MAX_DEPTH;

};