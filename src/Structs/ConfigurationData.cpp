#include "Structs/ConfigurationData.h"

void ConfigurationData::resetToDefault() {
#ifdef GERMAN_DEFAULT_LANGUAGE
	language = Language::Lang_DE;
#else
	language = Language::Lang_EN;
#endif
	isSoundOn = true;
	volumeSound = 100;
	volumeMusic = 100;
	isVSyncEnabled = true;
	isFPSLimited = false;
	maxFPS = 60;
	mainKeyMap = DEFAULT_KEYMAP;
	alternativeKeyMap = ALTERNATIVE_KEYMAP;
	joystickKeyMap = JOYSTICK_KEYMAP;
	displayMode = DisplayMode::Fullscreen;
	isQuickcast = true;
	isAutotarget = true;
	isSmoothing = true;
	isDisplayHints = true;
	isDisplayQuestMarkers = true;
	isMultithreading = true;
	isGodmode = false;
	isDebugRendering = false;
	isDebugRenderingOn = false;
	isWindowReload = false;
	isDisplayTime = false;
	logLevel = LogLevel::Error;
}

const std::map<Key, sf::Keyboard::Key> ConfigurationData::DEFAULT_KEYMAP =
{
	{ Key::Escape, sf::Keyboard::Escape },
	{ Key::Quickload, sf::Keyboard::F9 },
	{ Key::Quicksave, sf::Keyboard::F5 },
	{ Key::Inventory, sf::Keyboard::I },
	{ Key::CharacterInfo, sf::Keyboard::C },
	{ Key::Map, sf::Keyboard::M },
	{ Key::Journal, sf::Keyboard::J },
	{ Key::Spellbook, sf::Keyboard::K },
	{ Key::Interact, sf::Keyboard::E },
	{ Key::Confirm, sf::Keyboard::Return },
	{ Key::Left, sf::Keyboard::A },
	{ Key::Right, sf::Keyboard::D },
	{ Key::Up, sf::Keyboard::W },
	{ Key::Down, sf::Keyboard::S },
	{ Key::Jump, sf::Keyboard::Space },
	{ Key::Chop, sf::Keyboard::F },
	{ Key::FirstSpell, sf::Keyboard::Num1 },
	{ Key::SecondSpell, sf::Keyboard::Num2 },
	{ Key::ThirdSpell, sf::Keyboard::Num3 },
	{ Key::FourthSpell, sf::Keyboard::Num4 },
	{ Key::FifthSpell, sf::Keyboard::Num5 },
	{ Key::QuickSlot1, sf::Keyboard::Num6 },
	{ Key::QuickSlot2, sf::Keyboard::Num7 },
	{ Key::ToggleTooltips, sf::Keyboard::LAlt },
	{ Key::SwitchTarget, sf::Keyboard::Tab },
	{ Key::Screenshot, sf::Keyboard::F10 },
	{ Key::Debug, sf::Keyboard::F11 },
	{ Key::BackToCheckpoint, sf::Keyboard::B },
	{ Key::ToggleAutotarget, sf::Keyboard::P },
	{ Key::Attack, sf::Keyboard::KeyCount }
};

const std::map<Key, sf::Keyboard::Key> ConfigurationData::ALTERNATIVE_KEYMAP =
{
	// sf::Keyboard::KeyCount means "not set"
	{ Key::Escape, sf::Keyboard::KeyCount },
	{ Key::Quickload, sf::Keyboard::KeyCount },
	{ Key::Quicksave, sf::Keyboard::KeyCount },
	{ Key::Inventory, sf::Keyboard::KeyCount },
	{ Key::CharacterInfo, sf::Keyboard::KeyCount },
	{ Key::Map, sf::Keyboard::KeyCount },
	{ Key::Journal, sf::Keyboard::KeyCount },
	{ Key::Spellbook, sf::Keyboard::KeyCount },
	{ Key::Interact, sf::Keyboard::KeyCount },
	{ Key::Confirm, sf::Keyboard::KeyCount },
	{ Key::Left, sf::Keyboard::Left },
	{ Key::Right, sf::Keyboard::Right },
	{ Key::Up, sf::Keyboard::Up },
	{ Key::Down, sf::Keyboard::Down },
	{ Key::Jump, sf::Keyboard::KeyCount },
	{ Key::Chop, sf::Keyboard::KeyCount },
	{ Key::FirstSpell, sf::Keyboard::KeyCount },
	{ Key::SecondSpell, sf::Keyboard::KeyCount },
	{ Key::ThirdSpell, sf::Keyboard::KeyCount },
	{ Key::FourthSpell, sf::Keyboard::KeyCount },
	{ Key::FifthSpell, sf::Keyboard::KeyCount },
	{ Key::QuickSlot1, sf::Keyboard::KeyCount },
	{ Key::QuickSlot2, sf::Keyboard::KeyCount },
	{ Key::ToggleTooltips, sf::Keyboard::KeyCount },
	{ Key::SwitchTarget, sf::Keyboard::KeyCount },
	{ Key::Screenshot, sf::Keyboard::KeyCount },
	{ Key::Debug, sf::Keyboard::KeyCount },
	{ Key::BackToCheckpoint, sf::Keyboard::KeyCount },
	{ Key::ToggleAutotarget, sf::Keyboard::KeyCount },
	{ Key::Attack, sf::Keyboard::KeyCount }
};

const std::map<Key, int> ConfigurationData::JOYSTICK_KEYMAP =
{
	{ Key::Escape, 3 }, // 3 is Y / Up
	{ Key::Quickload, -1 },
	{ Key::Quicksave, -1 },
	{ Key::Inventory, -1 },
	{ Key::CharacterInfo, -1 },
	{ Key::Map, -1 },
	{ Key::Journal, -1 },
	{ Key::Spellbook, -1 },
	{ Key::Interact, 2 }, // 2 is X / Right
	{ Key::Confirm, 2 }, // 2 is X / Right
	{ Key::Left, -1 },
	{ Key::Right, -1 },
	{ Key::Up, -1 },
	{ Key::Down, -1 },
	{ Key::Jump, 0 }, // 0 is A / Jump
	{ Key::Chop, -1 },
	{ Key::FirstSpell, -1 },
	{ Key::SecondSpell, -1 },
	{ Key::ThirdSpell, -1 },
	{ Key::FourthSpell, -1 },
	{ Key::FifthSpell, -1 },
	{ Key::QuickSlot1, -1 },
	{ Key::QuickSlot2, -1 },
	{ Key::ToggleTooltips, -1 },
	{ Key::SwitchTarget, -1 },
	{ Key::Screenshot, -1 },
	{ Key::Debug, -1 },
	{ Key::BackToCheckpoint, -1 },
	{ Key::ToggleAutotarget, -1 },
	{ Key::Attack, 1 } // 1 is B / Attack
};
