#include "Level.h"

const int Level::NUM_LAYERS = 3;

Level::Level()
{


	//levelFilepath = "";

	// Dynamically allocate memory for vector arrays
	sprites = new std::vector<sf::Sprite>[NUM_LAYERS];
	textures = new std::vector<std::string>[NUM_LAYERS];
	images = new std::vector<sf::Image*>[NUM_LAYERS];
}

Level::~Level()
{
	delete[] sprites;
	delete[] textures;
	delete[] images;

	sprites = NULL;
	textures = NULL;
	images = NULL;
}

void Level::Draw(Layer layer, sf::RenderWindow* renderWindow)
{
	if(sprites[layer].size() != 0)
	{
		for(size_t i = 0; i < sprites[layer].size(); i++)
		{
			renderWindow->Draw(sprites[layer][i]);
		}
	}
}

bool Level::Load(std::string levelFilepath)
{
	// Load level xml file 
	rapidxml::file<> xmlFile(levelFilepath.c_str());
	rapidxml::xml_document<> xmlDoc;
	xmlDoc.parse<0>(xmlFile.data()); 

	// Background layer
	rapidxml::xml_node<> *backgroundLayer = xmlDoc.first_node()->first_node()->first_node();
	rapidxml::xml_node<> *backgroundItem = backgroundLayer->first_node()->first_node();
	if(!LoadLayer(BACKGROUND, backgroundItem))
	{
		std::cout << "bool Level::Load() error: failed to load background layer." << std::endl;
		return false;
	}

	// Entities layer
	rapidxml::xml_node<> *entitiesLayer = backgroundLayer->next_sibling();
	rapidxml::xml_node<> *entitiesItem = entitiesLayer->first_node()->first_node();

	// Platforms layer
	rapidxml::xml_node<> *platformLayer = entitiesLayer->next_sibling();
	rapidxml::xml_node<> *platformItem = platformLayer->first_node()->first_node();
	if(!LoadLayer(PLATFORMS, platformItem))
	{
		std::cout << "bool Level::Load() error: failed to load platform layer." << std::endl;
		return false;
	}

	// Foreground layer
	rapidxml::xml_node<> *foregroundLayer = platformLayer->next_sibling();
	rapidxml::xml_node<> *foregroundItem = foregroundLayer->first_node()->first_node();
	if(!LoadLayer(FOREGROUND, foregroundItem))
	{
		std::cout << "bool Level::Load() error: failed to load foreground layer." << std::endl;
		return false;
	}

	
	if(!ParseEntities(entitiesItem))
	{
		std::cout << "Level::Load(std::string levelFilepath) error: Failed to parse entities layer." << std::endl;
		return false;
	}
	

	return true;
}

bool Level::ParseEntities(rapidxml::xml_node<> *node)
{

	// Cycle through items
	while(node != NULL)
	{
		// Store a pointer to the properties
		rapidxml::xml_node<> *properties = node->first_node("CustomProperties")->first_node("Property");

		// Cycle through the properties
		while(properties != NULL)
		{
			// Look for "ScoreBoard" property
			if((std::string)properties->first_attribute("Name")->value() == "ScoreBoard")
			{
				float xPos = 0.0f, yPos = 0.0f;
				float xCentre = 0.0f, yCentre = 0.0f;
				float xOrigin = 0.0f, yOrigin = 0.0f;

				// Grab origin position data
				xOrigin = (float)atof(node->first_node("Origin")->first_node("X")->value());
				yOrigin = (float)atof(node->first_node("Origin")->first_node("Y")->value());

				// Grab centre position data
				xCentre = (float)atof(node->first_node("Position")->first_node("X")->value());
				yCentre = (float)atof(node->first_node("Position")->first_node("Y")->value());

				// Set actual x/y pos
				xPos = xCentre - xOrigin;
				yPos = yCentre - yOrigin;

				//Add scoreBoard point to scoreBoards vector
				sf::Vector2f scoreBoard = sf::Vector2f(xPos, yPos);
				scoreBoards.push_back(scoreBoard);
			}

			// Get next custom property
			properties = properties->next_sibling();
		}
		
		// Get next entities layer item
		node = node->next_sibling();
	}

	return true;
}

std::vector<sf::Vector2f> Level::GetScoreBoards()
{
	return scoreBoards;
}

bool Level::LoadLayer(Layer layer, rapidxml::xml_node<> *layerItem)
{
	while(layerItem != NULL)
	{
		if(IsVisible(layerItem))
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
			xPos = xScale * (xCentre - xOrigin);
			yPos = yScale * (yCentre - yOrigin);

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
			for(size_t i = 0; i < textures[layer].size(); i++)
			{
				// A duplicate has been found
				if(textureFilename == textures[layer][i])
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
				textures[layer].push_back(textureFilename);
				images[layer].push_back(textureImage);

				// Set texture
				if(!textureImage->LoadFromFile(textures[layer][index]))
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
			sprite.SetImage(*images[layer][index]);
			sprites[layer].push_back(sprite);
		}

		// Get next platform
		layerItem = layerItem->next_sibling();
	}

	return true;
}

bool Level::IsVisible(rapidxml::xml_node<> *layerItem)
{
	// Check that custom properties are defined firstly
	if(layerItem->first_node("CustomProperties")->first_node("Property") != NULL)
	{
		// Store a pointer to the properties since it exists
		rapidxml::xml_node<> *customProperties = layerItem->first_node("CustomProperties")->first_node("Property");

		// Cycle through the properties
		while(customProperties != NULL)
		{
			// Look for the "Invisible" tag to determine if it is invisible
			if((std::string)customProperties->first_attribute("Name")->value() == "Invisible")
			{
				return false;
			}

			customProperties = customProperties->next_sibling();
		}
	}

	return true;
}
