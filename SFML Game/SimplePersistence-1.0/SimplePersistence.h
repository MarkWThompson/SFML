#pragma once
#include <string>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "PugiXML\pugixml.hpp"
#include "PugiXML\pugiconfig.hpp"
#include <map>

// Macro for quickly grabbing the singleton instance
#define sPersistence SimplePersistence::GetInstance()

// Singleton design pattern - global access, created once and safely destroyed

class SimplePersistence
{
public:

	/** Grabs the singleton instance of the class. */
	static SimplePersistence& GetInstance();

	enum PersistenceType
	{
		Bool,
		Int,
		Float,
		String,
	};
	

	bool Save();
	bool Load();
	void Clear();
	void ClearNoSave();
	void Initialise();


	bool HasEntry(std::string key);
	bool EntryIsType(PersistenceType typeToTest, std::string key);
	bool RemoveEntry(std::string key);


	bool AddPersistenceObject(std::string objectName);
	bool SetPersistenceObject(std::string objectToChangeToName);
	bool RemovePersistenceObject(std::string objectName);
	bool HasPersistenceObject(std::string objectName);
	int NoOfEntriesInPersistenceObject(std::string objectName);
	int NoOfEntriesInPersistenceObject(std::string objectName, PersistenceType typeFilter);
	bool ClearPersistenceObject(std::string objectName);
	std::string CurrentObjectName();


	void SetBool(std::string key, bool value);
	void SetInt(std::string key, int value);
	void SetFloat(std::string key, float value);
	void SetString(std::string key, std::string value);


	bool GetBool(std::string key);
	int GetInt(std::string key);
	float GetFloat(std::string key);
	std::string GetString(std::string key);


private:

	//A hasEntry that is only used internally, saves computation of having to find the correct node twice, plug in the node and it will come out found and shiz.
	bool HasEntry(std::string key, pugi::xml_node &outNode);
	//a HasPersistenceObject that is only used internally, saves somputation of having to find the correct object twice. plug in the node and it will come out found and shiz
	bool HasPersistenceObject(std::string objectName,  pugi::xml_node &outObject);

	//The active persistence structure.
	pugi::xml_document activePersistenceStructure;
	//The current active persistence Object
	pugi::xml_node currentPersistenceObject;

	//This is set in EnsureDirectory.
	std::string directoryPath;
	//This is set in the constructor
	std::string fullFilePath;

	//If the persistence directory dosent exist, creates it. The directory path is defined in PersistenceOptions
	void EnsureDirectory();
	//If a persistence file dosen't exist, creates it, the persistence file name is defined in PersistenceOptions
	bool EnsurePersistenceFile();
	void CreateNewPersistenceFile(std::string filePath, pugi::xml_document &activeStructure);
	
	//An internal function, plug in a persistence type and it will return the string representation. NEEDS TO BE MAINTAINED WHEN NEW TYPES ARE ADDED
	std::string StringFromPersistenceType(PersistenceType type);


	//Returns SimplePersistences directory path as a string
	const std::string GetPersistenceFileDirectoryPath();

	//Returns SimplePersistences persistence file name as a string
	const std::string GetPersistenceFileName();

	SimplePersistence();
	SimplePersistence(SimplePersistence const&); // Don't implement in .cpp
	void operator=(SimplePersistence const&); // Don't implement in .cpp

	// Persistence File Path
	static const std::string PERSISTENCE_FILE_PATH;
	static const std::string PERSISTENCE_FILE_NAME;

	//whether or not the persistence file is managed automatically or not
	static const bool AUTOMATIC_INITIALISATION;

	//Debug messages toggle
	static const bool DEBUG_MODE;
};

