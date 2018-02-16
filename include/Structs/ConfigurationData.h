#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/Language.h"
#include "Enums/DisplayMode.h"
#include "Enums/Key.h"
#include "Enums/LogLevel.h"

// describes the configuration that can be modified by the user.
// it is loaded every time the game starts and stored in the resource manager.
struct ConfigurationData final {
	Language language;
	bool isSoundOn;
	int volumeSound;
	int volumeMusic;
	bool isVSyncEnabled;
	bool isFPSLimited;
	int maxFPS;
	std::map<Key, sf::Keyboard::Key> mainKeyMap;
	std::map<Key, sf::Keyboard::Key> alternativeKeyMap;
	DisplayMode displayMode = DisplayMode::Window;
	bool isQuickcast;
	bool isAutotarget;
	bool isSmoothing;
	bool isDisplayHints;
	bool isDisplayQuestMarkers;

	bool isMultithreading;
	bool isGodmode;
	bool isDebugRendering;
	bool isWindowReload;
	bool isDisplayTime;
	LogLevel logLevel;

public:
	void resetToDefault();

	static const std::map<Key, sf::Keyboard::Key> DEFAULT_KEYMAP;
	static const std::map<Key, sf::Keyboard::Key> ALTERNATIVE_KEYMAP;
};
