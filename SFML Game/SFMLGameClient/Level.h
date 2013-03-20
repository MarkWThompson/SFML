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
	Level();

	~Level();

	/** Layer IDs for accessing the vector arrays. */
	enum Layer { BACKGROUND, PLATFORMS, FOREGROUND };

	/** Renders the level objects. */
	void Draw(Layer layer, sf::RenderWindow* renderWindow);

	/** 
	 * Loads the level based on the server message.
	 * Returns true if successful, otherwise returns false.
	 * For more information about level loading and creation, 
	 * view the readme.txt which can be found in '../Levels/'. 
	 */
	bool Load(std::string levelFilepath);

private:
	/** Defines the number of layers the level is expected to load. */
	static const int NUM_LAYERS;

	/**
	 * Used by the Load() method in order to load a layer of objects into memory.
	 * Returns true if the process was successful, otherwise returns false.
	 */
	bool LoadLayer(Layer layer, rapidxml::xml_node<> *layerItem);

	/** Returns true if the texture is visible, otherwise returns false. */
	bool IsVisible(rapidxml::xml_node<> *layerItem);
	
	// Layer vector arrays
	std::vector<sf::Sprite>* sprites;		///< Stores the level objects for rendering
	std::vector<std::string>* textures;		///< Stores a list of the textures
	std::vector<sf::Image*>* images;		///< Stores the textures as images for rendering
};