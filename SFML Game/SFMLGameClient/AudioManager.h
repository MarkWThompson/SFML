#pragma once

#include <SFML\Audio.hpp>
#include <string>
#include "Assets.h"
#include <iostream>

// Macro for quickly grabbing the singleton instance
#define audioManager AudioManager::GetInstance()

class AudioManager
{
public:
	/** Grabs the singleton instance of the class. */
	static AudioManager& GetInstance();

	// Sounds
	sf::Sound mousePressedSFX;

	// Music
	sf::Music menuMusic;
	sf::Music gameMusic;
	
private:

	// Buffers
	sf::SoundBuffer mousePressedSFXBuffer;

	AudioManager();
	AudioManager(AudioManager const&); // Don't implement in .cpp
	void operator=(AudioManager const&); // Don't implement in .cpp
};
