#include "Level.h"

Level::Level()
{
	// Initialise spawnPoints vector
	sf::Vector2f defaultSpawnPoint = sf::Vector2f(0.0f, 0.0f);
	spawnPoints.push_back(defaultSpawnPoint);

	levelFilepath = "";
}

Level::~Level()
{
}

bool Level::Load(std::string levelFilepath)
{
	this->levelFilepath = levelFilepath;

	// Load level xml file
	rapidxml::file<> xmlFile(levelFilepath.c_str());
	rapidxml::xml_document<> xmlDoc;
	xmlDoc.parse<0>(xmlFile.data()); 

	// Background layer
	rapidxml::xml_node<> *backgroundLayer = xmlDoc.first_node()->first_node()->first_node();
	
	// Entities layer
	rapidxml::xml_node<> *entitiesLayer = backgroundLayer->next_sibling();
	rapidxml::xml_node<> *entitiesItem = entitiesLayer->first_node()->first_node();

	// Platforms layer
	rapidxml::xml_node<> *platformLayer = entitiesLayer->next_sibling();

	// Forground layer
	rapidxml::xml_node<> *foregroundLayer = platformLayer->next_sibling();

	// Bounds layer
	rapidxml::xml_node<> *boundsLayer = foregroundLayer->next_sibling();
	rapidxml::xml_node<> *boundsItem = boundsLayer->first_node()->first_node();

	if(!ParseEntities(entitiesItem))
	{
		std::cout << "Level::Load(std::string levelFilepath) error: Failed to parse entities layer." << std::endl;
		return false;
	}

	if(!ParseBounds(boundsItem))
	{
		std::cout << "Level::Load(std::string levelFilepath) error: Failed to parse bounds layer." << std::endl;
		return false;
	}

	return true;
}

bool Level::ParseBounds(rapidxml::xml_node<> *node)
{
	// Cycle through bounds
	while(node != NULL)
	{
		float xPos = 0.0f, yPos = 0.0f; // Top left corner
		float width = 0.0f, height = 0.0f;
		bool lethal = false;

		// Grab centre position data
		xPos = (float)atof(node->first_node("Position")->first_node("X")->value());
		yPos = (float)atof(node->first_node("Position")->first_node("Y")->value());

		// Grab dimensions
		width = (float)atof(node->first_node("Width")->value());
		height = (float)atof(node->first_node("Height")->value());

		// Store a pointer to the properties
		rapidxml::xml_node<> *properties = node->first_node("CustomProperties")->first_node("Property");

		// Cycle through the properties
		while(properties != NULL)
		{
			// Look for "Lethal" property
			if((std::string)properties->first_attribute("Name")->value() == "Lethal")
			{
				lethal = true;
			}

			// Get next custom property
			properties = properties->next_sibling();
		}

		// Create struct containing platform data and add to vector
		sf::Rect<float> bounds(xPos, yPos, xPos + width, yPos + height);
		collisionBounds.push_back(bounds);
		collisionBoundsLethality.push_back(lethal);

		// Get next bound
		node = node->next_sibling();
	}

	return true;
}

bool Level::ParseEntities(rapidxml::xml_node<> *node)
{
	bool spawnDefined = false;

	// Cycle through items
	while(node != NULL)
	{
		// Store a pointer to the properties
		rapidxml::xml_node<> *properties = node->first_node("CustomProperties")->first_node("Property");

		// Cycle through the properties
		while(properties != NULL)
		{
			// Look for "SpawnPoint" property
			if((std::string)properties->first_attribute("Name")->value() == "SpawnPoint")
			{
				float xPos = 0.0f, yPos = 0.0f;
				float xCentre = 0.0f, yCentre = 0.0f;
				float xOrigin = 0.0f, yOrigin = 0.0f;

				// Grab origin data
				xOrigin = (float)atof(node->first_node("Origin")->first_node("X")->value());
				yOrigin = (float)atof(node->first_node("Origin")->first_node("Y")->value());

				// Grab centre position data
				xCentre = (float)atof(node->first_node("Position")->first_node("X")->value());
				yCentre = (float)atof(node->first_node("Position")->first_node("Y")->value());

				// Set actual x/y pos
				xPos = xCentre - xOrigin;
				yPos = yCentre - yOrigin;

				// Remove default spawn point since the map defines spawn points
				if(!spawnDefined)
				{
					spawnPoints.pop_back(); // Removes default spawnpoint since custom ones are defined
					spawnDefined = true;
				}

				// Add spawn point to spawnPoints vector
				sf::Vector2f spawnPoint = sf::Vector2f(xPos, yPos);
				spawnPoints.push_back(spawnPoint);
			}

			// Look for "LevelBounds" property
			if((std::string)properties->first_attribute("Name")->value() == "LevelBounds")
			{
				float xPos = 0.0f, yPos = 0.0f; // Top left corner
				float width = 0.0f, height = 0.0f;

				// Grab centre position data
				xPos = (float)atof(node->first_node("Position")->first_node("X")->value());
				yPos = (float)atof(node->first_node("Position")->first_node("Y")->value());

				// Grab dimensions
				width = (float)atof(node->first_node("Width")->value());
				height = (float)atof(node->first_node("Height")->value());

				// Create rect structure containing platform data and add to vector
				sf::Rect<float> bounds(xPos, yPos, xPos + width, yPos + height);

				this->bounds = bounds;
			}

			// Get next custom property
			properties = properties->next_sibling();
		}
		
		// Get next foreground item
		node = node->next_sibling();
	}

	return true;
}

std::vector<sf::Rect<float>> Level::GetCollisionBounds()
{
	return collisionBounds;
}

std::vector<bool> Level::GetCollisionBoundsLethality()
{
	return collisionBoundsLethality;
}

std::vector<sf::Vector2f> Level::GetSpawnPoints()
{
	return spawnPoints;
}

sf::Rect<float> Level::GetBounds()
{
	return bounds;
}
