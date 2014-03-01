#include "AudioManager.h"

AudioManager::AudioManager()
{
	if(!mousePressedSFXBuffer.LoadFromFile(MOUSE_PRESSED_SFX))
	{
		std::cout << "AudioManager::AudioManager() error: Failed to load mouse SFX." << std::endl;
	}

	if(!menuMusic.OpenFromFile(MENU_MUSIC))
	{
		std::cout << "AudioManager::AudioManager() error: Failed to load menu music." << std::endl;
	}

	if(!gameMusic.OpenFromFile(GAME_MUSIC))
	{
		std::cout << "AudioManager::AudioManager() error: Failed to load game music." << std::endl;
	}

	menuMusic.SetLoop(true);
	gameMusic.SetLoop(true);

	mousePressedSFX.SetBuffer(mousePressedSFXBuffer);
}

AudioManager& AudioManager::GetInstance()
{
	// Instantiated on first use, reference passed thereafter
	static AudioManager instance;
	return instance;
}