#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <rapidxml-1.13\rapidxml.hpp>
#include <rapidxml-1.13\rapidxml_utils.hpp>
#include "ServerNetworkData.h"
#include "Assets.h"

class Level
{
public:
	/** Default constructor. */
	Level();
	
	/** Default destructor. */
	~Level();

	/** Renders the level objects. */
	void Draw(sf::RenderWindow &renderWindow);

	/** 
	 * Loads the level based on the server message.
	 * Returns true if successful, otherwise returns false.
	 * For more information about level loading and creation, 
	 * view the readme.txt which can be found in '../Levels/'. 
	 */
	bool Load(std::string levelFilepath);

private:

	/**
	 * Used by the Load() method in order to load a layer of objects into memory.
	 * Returns true if the process was successful, otherwise returns false.
	 */
	bool LoadLayer(rapidxml::xml_node<> *layerItem);

	std::vector<sf::Sprite> sprites;	///< Stores the level objects for rendering
	std::vector<std::string> textures;	///< Stores a list of the textures
	std::vector<sf::Image*> images;		///< Stores the textures as images for rendering
};