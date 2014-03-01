#pragma once

#include <string>

// Prefixes (relative to project folders) - roots
static const std::string ASSETS_PREFIX = "../Assets/";
static const std::string AUDIO_PREFIX = "../Audio/";

// Specific shorthand prefixes
static const std::string FONTS_PREFIX	= ASSETS_PREFIX + "Fonts/";
static const std::string STATES_PREFIX	= ASSETS_PREFIX + "States/";
static const std::string LEVELS_PREFIX	= ASSETS_PREFIX + "Levels/";
static const std::string UI_PREFIX		= ASSETS_PREFIX + "UI/";
static const std::string PLAYER_PREFIX	= ASSETS_PREFIX + "Player/";
static const std::string SPLASH_PREFIX	= STATES_PREFIX + "Splash/";
static const std::string LOBBY_PREFIX	= STATES_PREFIX + "Lobby/";
static const std::string CREDITS_PREFIX = STATES_PREFIX + "Credits/";
static const std::string OPTIONS_PREFIX = STATES_PREFIX + "Options/";
static const std::string GAME_PREFIX	= STATES_PREFIX + "Game/";
static const std::string HELP_PREFIX	= STATES_PREFIX + "Help/";
static const std::string MUSIC_PREFIX	= AUDIO_PREFIX + "Music/";
static const std::string SFX_PREFIX		= AUDIO_PREFIX + "SFX/";

// Icons & UI
#define ICON_IMAGE				(UI_PREFIX + "Icon.png")
#define CONNECTING_IMAGE		(UI_PREFIX + "Connecting.png")
#define INTERUPT_IMAGE			(UI_PREFIX + "Interruption.png")
#define OK_BUTTON_IMAGE			(UI_PREFIX + "OKButton.jpg")

// Menu UI
#define LOBBY_BUTTON_IMAGE		(STATES_PREFIX + "LobbyButton.png")
#define OPTIONS_BUTTON_IMAGE	(STATES_PREFIX + "OptionsButton.png")
#define HELP_BUTTON_IMAGE		(STATES_PREFIX + "HelpButton.png")
#define CREDITS_BUTTON_IMAGE	(STATES_PREFIX + "CreditsButton.png")
#define CONNECT_BUTTON_IMAGE	(LOBBY_PREFIX + "ConnectButton.jpg")
#define GENERAL_BUTTON_IMAGE	(OPTIONS_PREFIX + "GeneralButton.jpg")
#define AUDIO_BUTTON_IMAGE		(OPTIONS_PREFIX + "AudioButton.jpg")
#define DISPLAY_BUTTON_IMAGE	(OPTIONS_PREFIX + "DisplayButton.jpg")
#define CHANGE_BUTTON_IMAGE		(OPTIONS_PREFIX + "ChangeButton.jpg")

// Player
#define PLAYER_IMAGE			(PLAYER_PREFIX + "Full.png")
#define PLAYER_ARM_IMAGE		(PLAYER_PREFIX + "Arm.png")
#define PLAYER_BODY_IMAGE		(PLAYER_PREFIX + "Body.png")
#define PLAYER_HEAD_IMAGE		(PLAYER_PREFIX + "Head.png")

// Pause Menu
#define MENU_BUTTON_IMAGE		(GAME_PREFIX + "QuitToMenuButton.jpg")
#define RETURN_BUTTON_IMAGE		(GAME_PREFIX + "ReturnToGameButton.jpg")
#define PAUSE_BACKING_IMAGE		(GAME_PREFIX + "PauseBacking.png")

// Game Objects
#define PROJECTILE_IMAGE		(GAME_PREFIX + "Projectile.png")

// Game UI
#define CROSSHAIR_IMAGE				(GAME_PREFIX + "Crosshair.png")
#define HEALTH_BAR_BACKING_IMAGE	(GAME_PREFIX + "HealthBarBacking.png")
#define HEALTH_BAR_FILL_IMAGE		(GAME_PREFIX + "HealthBarFill.png")
#define HIGHSCOREFRAME_IMAGE		(GAME_PREFIX + "ScoreBoardFrame.png")

// State Overlays
#define LOBBY_OVERLAY_IMAGE		(LOBBY_PREFIX + "Overlay.png")
#define OPTIONS_OVERLAY_IMAGE	(OPTIONS_PREFIX + "Overlay.png")
#define HELP_OVERLAY_IMAGE		(HELP_PREFIX + "Overlay.png")
#define CREDITS_OVERLAY_IMAGE	(CREDITS_PREFIX + "Overlay.png")

// Backgrounds
#define SPLASH_BG_IMAGE			(SPLASH_PREFIX + "Background.jpg")
#define SPLASH_MESSAGE_IMAGE	(SPLASH_PREFIX + "Instruction.png")
#define MENU_BG_IMAGE			(STATES_PREFIX + "Background.jpg")

// Fonts
#define ARIAL_FONT_FILE			(FONTS_PREFIX + "arial.ttf")
#define MOIRE_FONT_FILE			(FONTS_PREFIX + "Moire-Regular.ttf")

// Music
#define MENU_MUSIC				(MUSIC_PREFIX + "Menu.ogg")
#define GAME_MUSIC				(MUSIC_PREFIX + "Game.ogg")

// SFX
#define MOUSE_PRESSED_SFX		(SFX_PREFIX + "MousePressed.ogg")
