#include "Level.h"

Level::Level()
{
}

Level::~Level()
{
}

void Level::Draw(sf::RenderWindow &renderWindow)
{
	for(int i = 0; i < sprites.size(); i++)
	{
		renderWindow.Draw(sprites[i]);
	}
}

bool Level::Load(std::string levelFilepath)
{
	// Load level xml file 
	rapidxml::file<> xmlFile(levelFilepath.c_str());
	rapidxml::xml_document<> xmlDoc;
	xmlDoc.parse<0>(xmlFile.data()); 

	// Get to the layer nodes
	rapidxml::xml_node<> *layerNode = xmlDoc.first_node()->first_node()->first_node();

	// Background layer
	rapidxml::xml_node<> *background = layerNode->first_node()->first_node(); // Get to the item nodes (background)
	if(!LoadLayer(background))
	{
		std::cout << "bool Level::Load() error: failed to load background layer." << std::endl;
		return false;
	}
	layerNode = layerNode->next_sibling(); // Get to the next layer (platforms layer)

	// Platforms layer
	rapidxml::xml_node<> *platform = layerNode->first_node()->first_node(); // Get to the item nodes (platforms)
	if(!LoadLayer(platform))
	{
		std::cout << "bool Level::Load() error: failed to load platform layer." << std::endl;
		return false;
	}

	return true;
}

bool Level::LoadLayer(rapidxml::xml_node<> *layerItem)
{
	while(layerItem != NULL)
	{
		float xPos = 0.0f, yPos = 0.0f;
		float xCentre = 0.0f, yCentre = 0.0f;
		float xOrigin = 0.0f, yOrigin = 0.0f;
		float xScale = 0.0f, yScale = 0.0f;
		std::string textureFilename = ASSETS_PREFIX;
		sf::Image* textureImage = new sf::Image();
		std::stringstream buffer;

		// Grab origin data
		xOrigin = (float)atof(layerItem->first_node("Origin")->first_node("X")->value());
		yOrigin = (float)atof(layerItem->first_node("Origin")->first_node("Y")->value());

		// Grab centre position data
		xCentre = (float)atof(layerItem->first_node("Position")->first_node("X")->value());
		yCentre = (float)atof(layerItem->first_node("Position")->first_node("Y")->value());

		// Grab scaling data
		xScale = (float)atof(layerItem->first_node("Scale")->first_node("X")->value());
		yScale = (float)atof(layerItem->first_node("Scale")->first_node("Y")->value());

		// Set actual x/y pos
		xPos = xCentre - xOrigin;
		yPos = yCentre - yOrigin;

		// Grab texture data
		buffer << layerItem->first_node("texture_filename")->value();
		textureFilename += buffer.str();
		
		// Found duplicate texture in levelTextures vector
		bool duplicate = false; 

		/** 
		 * Used to load the texture for the sprite.
		 * Found: the index at which the duplicate texture was found in the vector
		 * Not found: the last index of the vector
		 */
		int index = 0;
		
		// Check for duplicate textures
		for(int i = 0; i < textures.size(); i++)
		{
			// A duplicate has been found
			if(textureFilename == textures[i])
			{
				duplicate = true;
				break;
			}

			index++;
		}

		// Prevent several textures which are the same from being stored in memory
		if(!duplicate)
		{
			// Update vectors with the new texture
			textures.push_back(textureFilename);
			images.push_back(textureImage);

			// Set texture
			if(!textureImage->LoadFromFile(textures[index]))
			{
				std::cout << "bool Level::LoadLayer(rapidxml::xml_node<> *layerItem) error: failed to load object texture." << std::endl;
				return false;
			}
		}
		else
		{
			// Free memory since a new texture is not needed
			delete textureImage;
			textureImage = NULL;
		}

		// Create the object as a sprite and add to the levelSprites vector
		sf::Sprite sprite;
		sprite.SetScale(xScale, yScale);
		sprite.SetPosition(xPos, yPos);
		sprite.SetImage(*images[index]);
		sprites.push_back(sprite);

		// Get next platform
		layerItem = layerItem->next_sibling();
	}

	return true;
}