#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/Language.h"
#include "Enums/DisplayMode.h"
#include "Enums/Key.h"

// describes the configuration that can be modified by the user.
// it is loaded every time the game starts and stored in the resource manager.
struct ConfigurationData final {
	Language language = Language::Lang_EN;
	bool isSoundOn = true;
	int volumeSound = 100;
	int volumeMusic = 100;
	bool isVSyncEnabled = true;
	bool isFPSLimited = false;
	int maxFPS = 60;
	std::map<Key, sf::Keyboard::Key> mainKeyMap = DEFAULT_KEYMAP;
	std::map<Key, sf::Keyboard::Key> alternativeKeyMap = ALTERNATIVE_KEYMAP;
	DisplayMode displayMode = DisplayMode::Window;
	bool isQuickcast = true;
	bool isAutotarget = true;
	bool isSmoothing = true;
	bool isDisplayHints = true;
	bool isDisplayQuestMarkers = true;

	bool isDisplayStopwatch = false;
	bool isGodmode = false;
	bool isDebugRendering = false;
	bool isWindowReload = false;

public:
	void resetToDefault();

	static const std::map<Key, sf::Keyboard::Key> DEFAULT_KEYMAP;
	static const std::map<Key, sf::Keyboard::Key> ALTERNATIVE_KEYMAP;
};
