#include "SimplePersistence.h"


// Define Default Persistence File Path
const std::string SimplePersistence::PERSISTENCE_FILE_PATH = "..\\Data";
//Define Default Persistence File Name
const std::string SimplePersistence::PERSISTENCE_FILE_NAME = "Settings";

//Define whether or not SimplePersistence automatically initialises itslef when needed, or requires the programmer to call Init() before file checking is done. Turning this to false is dangerous and could cause unexpected behaviour, but could be useful when you are concerned with performence.
const bool SimplePersistence::AUTOMATIC_INITIALISATION = true;

//Define whether we print out the super annoying "debug" text
const bool SimplePersistence::DEBUG_MODE = false;

SimplePersistence& SimplePersistence::GetInstance()
{
	// Instantiated on first use, reference passed thereafter
	static SimplePersistence instance;
	return instance;
}

SimplePersistence::SimplePersistence(void)
{
	if(AUTOMATIC_INITIALISATION)
	{
		fullFilePath =  GetPersistenceFileDirectoryPath() + "\\" + GetPersistenceFileName() + ".xml";

		EnsureDirectory();
		EnsurePersistenceFile();
		Load();
	}
	else
	{
		if(DEBUG_MODE)
		{
			std::cout << "Automatic Initialisation is disabled, be aware, not neccesarily an error." << std::endl;
		}
	}
}



//-- Saving and loading from File --//

//Saves the entire structure to file
bool SimplePersistence::Save()
{
	// save document to file
	if(DEBUG_MODE)
	{
		std::cout << "Saving Persistence Structure" << std::endl;
	}

	//pugi::xml_document clearDoc;
	//clearDoc.save_file(fullFilePath.c_str());
	if(activePersistenceStructure.save_file(fullFilePath.c_str()))
	{
		return true;
	}
	else
	{
		if(DEBUG_MODE)
		{
			std::cout << "Save Failed." << std::endl;
		}
		return false;
	}
}

//Loads the xml file into the active persistence structure
bool SimplePersistence::Load()
{
	ClearNoSave();

	pugi::xml_parse_result result = activePersistenceStructure.load_file(fullFilePath.c_str());
	if(DEBUG_MODE)
	{
		std::cout << "Load result: " << result.description() << std::endl;
	}

	if(result)
	{
		//Make sure the is a default
		pugi::xml_node defaultNode = activePersistenceStructure.child("Default");
		if(defaultNode != NULL)
		{
			currentPersistenceObject = activePersistenceStructure.child("Default");
		}
		else
		{
			if(DEBUG_MODE)
			{
				std::cout << ("There is no default persistance object, adding one") << std::endl;
			}
			AddPersistenceObject("Default");
		}
		return true;
	}
	else
	{
		if(DEBUG_MODE)
		{
			std::cout << "Load Failed." << std::endl;
		}
		return false;
	}

}

//Clears the entire structure, and saves to XML file
void SimplePersistence::Clear()
{
	if(DEBUG_MODE)
	{
		std::cout << ("Clearing Persistance structure to empty and saving") << std::endl;
	}

	//Clear the active structure, ready to be reinitialised
	activePersistenceStructure.reset();

	//add Default PersistenceObject Node
	AddPersistenceObject("Default");
	Save();
}

//Clears the entire stucture, but dosent save to the XML file
void SimplePersistence::ClearNoSave()
{
	if(DEBUG_MODE)
	{
		std::cout << ("Clearing Persistance structure to empty, not saving") << std::endl;
	}

	//Clear the active structure, ready to be reinitialised
	activePersistenceStructure.reset();

	//add Default PersistenceObject Node
	AddPersistenceObject("Default");
}

//Runs validation on the persistence directory/file. Neccesary to be called before any sPersistence use if automatic initialisation is disabled
void SimplePersistence::Initialise()
{
	fullFilePath =  GetPersistenceFileDirectoryPath() + "\\" + GetPersistenceFileName() + ".xml";

	EnsureDirectory();
	EnsurePersistenceFile();
	Load();
}


//-- Utility --//

//Checks if the current persistence structure has entry key
bool SimplePersistence::HasEntry(std::string key)
{
	//Check if the key already exists in the current persistance object
	pugi::xml_node entryNode;
	entryNode = currentPersistenceObject.child(key.c_str());

	if(entryNode.name() == key)
	{
		//Then the entry exists, return true
		return true;

	}
	else
	{
		//If the key entry dosent exist, report back
		if(DEBUG_MODE)
		{
			std::cout << ("Entry ") << key << (" dosen't exist") << std::endl;
		}
		return false;

	}
}

//Checks if entry key in the current persistence object is of type typeToText
bool SimplePersistence::EntryIsType(PersistenceType typeToTest, std::string key)
{
	//Check if the key already exists in the current persistance object
	pugi::xml_node entryNode;
	entryNode = currentPersistenceObject.child(key.c_str());

	if(HasEntry(key,entryNode))
	{
		if(entryNode.attribute("type").as_string() == StringFromPersistenceType(typeToTest))
		{
			//If the type is the same type as the inputted type to test, return true
			return true;
		}
		else
		{
			if(DEBUG_MODE)
			{
				std::cout << ("Entry ") << key << (" is not of type ") << StringFromPersistenceType(typeToTest) << (", returning false") << std::endl;
			}
			return false;
		}

	}
	else
	{
		//If the key entry dosent exist, report back
		if(DEBUG_MODE)
		{
			std::cout << ("Entry ") << key << (" dosen't exist, cannot check type, returning false") << std::endl;
		}
		return false;

	}
}

//Removes an Entry from the active persistence object
bool SimplePersistence::RemoveEntry(std::string key)
{
	pugi::xml_node entryToDeleteNode;

	if(HasEntry(key,entryToDeleteNode))
	{
		if(DEBUG_MODE)
		{
			std::cout << "Removing Entry " << key << std::endl;
		}
		if(currentPersistenceObject.remove_child(entryToDeleteNode))
		{
			return true;
		}
		else
		{
			if(DEBUG_MODE)
			{
				std::cout << "Removing entry " << key << " failed for some reason" << std::endl;
			}
			return false;
		}
	}
	else
	{
		//The entry dosen't exist in the active persistence object, do nothing
		if(DEBUG_MODE)
		{
			std::cout << "The entry " << key << " dosen't exist in active persistence object, cannot delete it." << std::endl;
		}
		return false;
	}
}


//-- Dealing with Persistence Objects --//

//Adds a persistence object to the persistence structue, must call save to have this written to the file. All calls to set and get entries will be done only within the scope of the acive persistence object, changed via SetPersistenceObject
bool SimplePersistence::AddPersistenceObject(std::string objectName)
{
	pugi::xml_node persistenceStructureNode = activePersistenceStructure.child(objectName.c_str());

	if(persistenceStructureNode.name() == objectName)
	{
		//The object already exists, do nothing
		if(DEBUG_MODE)
		{
			std::cout << "Persistence Object " <<  objectName << " already exists, doing nothing" << std::endl;
		}
		return true;
	}
	else
	{
		//The object dosen't exist, so add it
		if(DEBUG_MODE)
		{
			std::cout << "Adding Persistence Object : " << objectName << std::endl;
		}
		currentPersistenceObject = activePersistenceStructure.append_child(objectName.c_str());
		
		if(currentPersistenceObject.empty())
		{
			//the object is empty, it's an error
			if(DEBUG_MODE)
			{
				std::cout << "Something went wrong adding the persistence object " << objectName << std::endl;
			}
			return false;
		}
		else
		{
			//else if the object isnt empty, we should be cool
			return true;
		}
	}
}

//Changes the current persistence object to the inputted name, if it exists
bool SimplePersistence::SetPersistenceObject(std::string objectToChangeToName)
{
	pugi::xml_node persistenceObjectNode = activePersistenceStructure.child(objectToChangeToName.c_str());

	if(persistenceObjectNode.name() == objectToChangeToName)
	{
		//The object already exists, so switch to it
		if(DEBUG_MODE)
		{
			std::cout << "Switching to Persistence Object " <<  objectToChangeToName << std::endl;
		}
		currentPersistenceObject = persistenceObjectNode;
		if(currentPersistenceObject.empty())
		{
			if(DEBUG_MODE)
			{
				std::cout << "Something went wrong switching to object " << objectToChangeToName << std::endl;
			}
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		//The object dosen't exist, so do nothing
		if(DEBUG_MODE)
		{
			std::cout << "Persistence Object " << objectToChangeToName << " dosen't exist, so we cannot switch to it" << std::endl;
		}
		return false;
	}
}

//Removes a persistence object from the structure, will also remove any entries that are in the object
bool SimplePersistence::RemovePersistenceObject(std::string objectName)
{
	pugi::xml_node objectToDeleteNode;

	if(HasPersistenceObject(objectName,objectToDeleteNode))
	{
		if(DEBUG_MODE)
		{
			std::cout << "Removing Persistence Object " << objectName << std::endl;
		}
		if(activePersistenceStructure.remove_child(objectToDeleteNode))
		{
			if(objectName == CurrentObjectName())
			{
				if(DEBUG_MODE)
				{
					std::cout << "Removing the current active persistence object, switching to default object" << std::endl;
				}
				if(HasPersistenceObject("Default"))
				{
					SetPersistenceObject("Default");
				}
				else
				{
					AddPersistenceObject("Default");
					SetPersistenceObject("Default");
				}
			}
			return true;
		}
		else
		{
			if(DEBUG_MODE)
			{
				std::cout << "Something went wrong with removing object " << objectName << std::endl;
			}
			return false;
		}
	}
	else
	{
		//The object dosen't exist in the structure, do nothing
		if(DEBUG_MODE)
		{
			std::cout << "The persistence object " << objectName << " dosen't exist, cannot delete it." << std::endl;
		}
		return false;
	}
}

//Checks if there is a persistance object objectName in the structure
bool SimplePersistence::HasPersistenceObject(std::string objectName)
{
	//Check if the key already exists in the current persistance object
	pugi::xml_node persistenceObjectNode;
	persistenceObjectNode = activePersistenceStructure.child(objectName.c_str());

	if(persistenceObjectNode.name() == objectName)
	{
		//Then the Object exists, return true
		return true;

	}
	else
	{
		//If the persistence dosent exist, report back
		if(DEBUG_MODE)
		{
			std::cout << ("Persistence Object ") << objectName << (" dosen't exist") << std::endl;
		}
		return false;

	}
}

//Returns the number of entries in entered persistence object, if it exists.
int SimplePersistence::NoOfEntriesInPersistenceObject(std::string objectName)
{
	pugi::xml_node persistenceObject;
	if(HasPersistenceObject(objectName,persistenceObject))
	{
		//The persistenceObject exists, we can find how many entries are in it

		//Get the first child, and then traverse the object counting how many entries there are
		int entryCount = 0;

		pugi::xml_node firstChild = persistenceObject.first_child();
		pugi::xml_node nextChild = persistenceObject.first_child();
		if(firstChild == NULL)
		{
			//There are no entries in this object
			return 0;
		}
		else
		{
			while(nextChild != NULL)
			{
				entryCount++;
				nextChild = nextChild.next_sibling();
			}

			return entryCount;
		}
	}
	else
	{
		//It dosent exist, do nothing
		if(DEBUG_MODE)
		{
			std::cout << "Object " << objectName << " dosen't exist, thus has 0 entries, returning 0";
		}
		return 0;
	}
}

//Returns the number of entries in the persistence object of type typeFilter
int SimplePersistence::NoOfEntriesInPersistenceObject(std::string objectName, PersistenceType typeFilter)
{
	pugi::xml_node persistenceObject;
	if(HasPersistenceObject(objectName,persistenceObject))
	{
		//The persistenceObject exists, we can find how many entries are in it

		//Get the first child, and then traverse the object counting how many entries there are
		int entryCount = 0;

		pugi::xml_node firstChild = persistenceObject.first_child();
		pugi::xml_node nextChild = persistenceObject.first_child();
		if(firstChild == NULL)
		{
			//There are no entries in this object
			return 0;
		}
		else
		{
			while(nextChild != NULL)
			{
				if(nextChild.attribute("type").as_string() == StringFromPersistenceType(typeFilter))
				{
					entryCount++;
				}
				nextChild = nextChild.next_sibling();
			}

			return entryCount;
		}
	}
	else
	{
		//It dosent exist, do nothing
		if(DEBUG_MODE)
		{
			std::cout << "Object " << objectName << " dosen't exist, thus has 0 entries, returning 0";
		}
		return 0;
	}
}

bool SimplePersistence::ClearPersistenceObject(std::string objectName)
{
	pugi::xml_node objectToClear;
	if(HasPersistenceObject(objectName,objectToClear))
	{
		//While the object has a child, Kill the fucker
		while(objectToClear.first_child())
		{
			objectToClear.remove_child(objectToClear.first_child());
		}
		return true;
	}
	else
	{
		if(DEBUG_MODE)
		{
			std::cout << "Object " << objectName << " dosen't exist, cannot clear" << std::endl;
		}
		return false;
	}
}

//Returns the active persistence object name as a string
std::string SimplePersistence::CurrentObjectName()
{
	return currentPersistenceObject.name();
}


//-- Setters --//

//Saves an Bool into the active persistence object.
void SimplePersistence::SetBool(std::string key, bool value)
{
	//Check if the key already exists in the current persistance object
	pugi::xml_node boolEntryNode;

	if(HasEntry(key,boolEntryNode))
	{
		//The entry already exists, just modify the value
		pugi::xml_attribute boolValueToModify = boolEntryNode.attribute("value");
		boolValueToModify = value;
	}
	else
	{
		//If the key entry dosent exist, create it

		// param node for the entry
		pugi::xml_node boolEntry = currentPersistenceObject.append_child(key.c_str());

		// add attributes to param node
		boolEntry.append_attribute("type") = "Bool";
		boolEntry.append_attribute("value") = value;
	}

	
}

//Saves an Int into the active persistence object.
void SimplePersistence::SetInt(std::string key, int value)
{
	//Check if the key already exists in the current persistance object
	pugi::xml_node intEntryNode;

	if(HasEntry(key,intEntryNode))
	{
		//The entry already exists, just modify the value
		pugi::xml_attribute intValueToModify = intEntryNode.attribute("value");
		intValueToModify = value;
	}
	else
	{
		//If the key entry dosent exist, create it

		// param node for the entry
		pugi::xml_node intEntry = currentPersistenceObject.append_child(key.c_str());

		// add attributes to param node
		//intEntry.append_attribute("key") = key.c_str();
		intEntry.append_attribute("type") = "Int";
		intEntry.append_attribute("value") = value;
	}

	
}

//Saves an Float into the active persistence object.
void SimplePersistence::SetFloat(std::string key, float value)
{
	//Check if the key already exists in the current persistance object
	pugi::xml_node floatEntryNode;

	if(HasEntry(key,floatEntryNode))
	{
		//The entry already exists, just modify the value
		pugi::xml_attribute floatValueToModify = floatEntryNode.attribute("value");
		floatValueToModify = value;
	}
	else
	{
		//If the key entry dosent exist, create it

		// param node for the entry
		pugi::xml_node floatEntry = currentPersistenceObject.append_child(key.c_str());

		// add attributes to param node
		//intEntry.append_attribute("key") = key.c_str();
		floatEntry.append_attribute("type") = "Float";
		floatEntry.append_attribute("value") = value;
	}

	
}

//Saves an String into the active persistence object.
void SimplePersistence::SetString(std::string key, std::string value)
{
	//Check if the key already exists in the current persistance object
	pugi::xml_node stringEntryNode;

	if(HasEntry(key,stringEntryNode))
	{
		//The entry already exists, just modify the value
		pugi::xml_attribute stringValueToModify = stringEntryNode.attribute("value");
		stringValueToModify = value.c_str();
	}
	else
	{
		//If the key entry dosent exist, create it

		// param node for the entry
		pugi::xml_node stringEntry = currentPersistenceObject.append_child(key.c_str());

		// add attributes to param node
		stringEntry.append_attribute("type") = "String";
		stringEntry.append_attribute("value") = value.c_str();
	}

	
}


//-- Getters --//

//Gets the value of an Bool of name key in active persistence object, will fail if entry dosent exist or isnt an Bool
bool SimplePersistence::GetBool(std::string key)
{
	//EntryIsType uses HasEntry, so you're going to get three cascading error messages here if it fails, you can just turn them off though.

	//EntryIsType checks for HasEntry(), no need to worry here
	if(EntryIsType(PersistenceType::Bool,key))
	{
		//If the key type is an int, then we can return it
		pugi::xml_node boolEntryNode;
		boolEntryNode = currentPersistenceObject.child(key.c_str());
		return boolEntryNode.attribute("value").as_bool();
	}
	else
	{
		//Its not an bool, SO YOU CANT FUCKING RETURN IT GODDAMN IT ROSS!
		if(DEBUG_MODE)
		{
			std::cout << "ERROR : Entry is not an bool or does not exist, returning false" << std::endl;
		}
		return false;
	}
}

//Gets the value of an Int of name key in active persistence object, will fail if entry dosent exist or isnt an Int
int SimplePersistence::GetInt(std::string key)
{
	//EntryIsType uses HasEntry, so you're going to get three cascading error messages here if it fails, you can just turn them off though.

	//EntryIsType checks for HasEntry(), no need to worry here
	if(EntryIsType(PersistenceType::Int,key))
	{
		//If the key type is an int, then we can return it
		pugi::xml_node intEntryNode;
		intEntryNode = currentPersistenceObject.child(key.c_str());
		return intEntryNode.attribute("value").as_int();
	}
	else
	{
		//Its not an int, SO YOU CANT FUCKING RETURN IT GODDAMN IT ROSS!
		if(DEBUG_MODE)
		{
			std::cout << "ERROR : Entry is not an int or does not exist, returning 0" << std::endl;
		}
		return 0;
	}
}

//Gets the value of an Float of name key in active persistence object, will fail if entry dosent exist or isnt an Float
float SimplePersistence::GetFloat(std::string key)
{
	//EntryIsType uses HasEntry, so you're going to get three cascading error messages here if it fails, you can just turn them off though.

	//EntryIsType checks for HasEntry(), no need to worry here
	if(EntryIsType(PersistenceType::Float,key))
	{
		//If the key type is an float, then we can return it
		pugi::xml_node floatEntryNode;
		floatEntryNode = currentPersistenceObject.child(key.c_str());
		return floatEntryNode.attribute("value").as_float();
	}
	else
	{
		//Its not an float, SO YOU CANT FUCKING RETURN IT GODDAMN IT ROSS!
		if(DEBUG_MODE)
		{
			std::cout << "ERROR : Entry is not an float or does not exist, returning 0" << std::endl;
		}
		return 0.0f;
	}
}


//Gets the value of an String of name key in active persistence object, will fail if entry dosent exist or isnt an String
std::string SimplePersistence::GetString(std::string key)
{
	//EntryIsType uses HasEntry, so you're going to get three cascading error messages here if it fails, you can just turn them off though.

	//EntryIsType checks for HasEntry(), no need to worry here
	if(EntryIsType(PersistenceType::String,key))
	{
		//If the key type is an string, then we can return it
		pugi::xml_node stringEntryNode;
		stringEntryNode = currentPersistenceObject.child(key.c_str());
		return stringEntryNode.attribute("value").as_string();
	}
	else
	{
		//Its not an float, SO YOU CANT FUCKING RETURN IT GODDAMN IT ROSS!
		if(DEBUG_MODE)
		{
			std::cout << "ERROR : Entry is not an string or does not exist, returning an empty string" << std::endl;
		}
		return "";
	}
}


//-- Internal Utility --//

//Ensures that there is a persistence directory to work with
void SimplePersistence::EnsureDirectory()
{
	//Create the directory for the persistence file, if it is already there _mkdir will fail
	//_mkdir returns 0 if the directory is created succesfully, else it fails, hence the int
	int reportDirectoryCreation;

	std::string directoryPath = GetPersistenceFileDirectoryPath();

	reportDirectoryCreation = _mkdir(directoryPath.c_str());
	if (!reportDirectoryCreation)
		std::cout << ("New persistence directory created at : ") << directoryPath << std::endl;
	else
	{
		std::cout << ("Persistence directory already exists. \n");
		//No new directory was created
	}

	this->directoryPath = directoryPath;
}

//Ensures that there is a persistence file in the persistence directory
bool SimplePersistence::EnsurePersistenceFile()
{
	std::string filePath = fullFilePath;

	//If ifile exists, then we know it exists, and as such, it existingly exists
	std::ifstream ifile(filePath.c_str());
	if (ifile) 
	{
	  // The file exists, and is open for input
		std::cout << ("Persistence file already exists.") << std::endl;
		return true;
	}
	else
	{
		//there is no file
		std::cout << ("Persistence file dosen't exist, creating one.") << std::endl;
		CreateNewPersistenceFile(filePath,activePersistenceStructure);
		return false;
	}
}

//Creates a brand new persistence file in the correct directory
void SimplePersistence::CreateNewPersistenceFile(std::string filePath, pugi::xml_document &activeStructure)
{
	if(DEBUG_MODE)
	{
		std::cout << ("Creating new persistence file.") << std::endl;
	}

	//Clear the active structure, ready to be reinitialised
	activeStructure.reset();

	//add Default PersistenceObject Node
	AddPersistenceObject("Default");
	Save();
}

//this version of hasEntry also checks if the entry is there, but allows you to plug in a node and have it turned into the node where the entry is. Good so you dont have to find the node twice in some cases
bool SimplePersistence::HasEntry(std::string key, pugi::xml_node &outNode)
{
	outNode = currentPersistenceObject.child(key.c_str());

	if(outNode.name() == key)
	{
		//Then the entry exists, return true
		return true;

	}
	else
	{
		//If the key entry dosent exist, report back
		if(DEBUG_MODE)
		{
			std::cout << ("Entry ") << key << (" dosen't exist") << std::endl;
		}
		return false;
	}
}

//This version of hasPersistenceObject also check for the object, but allows you to plug in a node and have that turned into the node where the persistence object is
bool SimplePersistence::HasPersistenceObject(std::string objectName,  pugi::xml_node &outObject)
{
	//Check if the key already exists in the current persistance object
	outObject = activePersistenceStructure.child(objectName.c_str());

	if(outObject.name() == objectName)
	{
		//Then the Object exists, return true
		return true;
	}
	else
	{
		//If the persistence dosent exist, report back
		if(DEBUG_MODE)
		{
			std::cout << ("Persistence Object ") << objectName << (" dosen't exist") << std::endl;
		}
		return false;
	}
}

//Plug in a persistence type and it returns a string of it, neccesary for looking things up, needs to be maintained if you add a new type to save
std::string SimplePersistence::StringFromPersistenceType(PersistenceType type)
{
	//This array must be the same size as the PersistenceType Enum
	std::string  EnumStrings[] = { "Bool", "Int", "Float","String" };

	return EnumStrings[type];
}

const std::string SimplePersistence::GetPersistenceFileDirectoryPath()
{
	return PERSISTENCE_FILE_PATH;
}

const std::string SimplePersistence::GetPersistenceFileName()
{
	return PERSISTENCE_FILE_NAME;
}

