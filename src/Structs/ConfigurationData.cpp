#include "Structs/ConfigurationData.h"
#include "Controller/GamepadMappings/GamepadMappings.h"
#include "Controller/GamepadController.h"

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
	gamepadProductId = GamepadController::getCurrentGamepadProductId();
	gamepadKeyMap = GamepadMappings::getDefaultMappings(gamepadProductId);
#ifdef DEBUG
	displayMode = DisplayMode::Window;
#else
	displayMode = DisplayMode::Fullscreen;
#endif
	isQuickcast = true;
	isAutotarget = true;
	isPauseInventory = false;
	isSmoothing = true;
	isDisplayHints = true;
	isDisplayQuestMarkers = true;
	isMultithreading = true;
	isGodmode = false;
#ifdef DEBUG
	isDebugRendering = true;
#else
	isDebugRendering = false;
#endif
	isDebugRenderingOn = false;
	isWindowReload = false;
	isDisplayTime = false;
#ifdef DEBUG
	logLevel = LogLevel::Info;
#else
	logLevel = LogLevel::Error;
#endif
}

void ConfigurationData::reloadGamepadMapping(GamepadProductID id) {
	gamepadProductId = GamepadController::getCurrentGamepadProductId();
	gamepadKeyMap = GamepadMappings::getDefaultMappings(gamepadProductId);
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
	{ Key::Move_Left, sf::Keyboard::A },
	{ Key::Move_Right, sf::Keyboard::D },
	{ Key::Move_Up, sf::Keyboard::W },
	{ Key::Move_Down, sf::Keyboard::S },
	{ Key::Jump, sf::Keyboard::Space },
	{ Key::Chop, sf::Keyboard::F },
	{ Key::FirstSpell, sf::Keyboard::Num1 },
	{ Key::SecondSpell, sf::Keyboard::Num2 },
	{ Key::ThirdSpell, sf::Keyboard::Num3 },
	{ Key::FourthSpell, sf::Keyboard::Num4 },
	{ Key::FifthSpell, sf::Keyboard::Num5 },
	{ Key::QuickSlot1, sf::Keyboard::Num6 },
	{ Key::QuickSlot2, sf::Keyboard::Num7 },
	{ Key::PreviousSpell, sf::Keyboard::R },
	{ Key::NextSpell, sf::Keyboard::T },
	{ Key::ToggleTooltips, sf::Keyboard::LAlt },
	{ Key::SwitchTarget, sf::Keyboard::Tab },
	{ Key::Screenshot, sf::Keyboard::F10 },
	{ Key::Debug, sf::Keyboard::F11 },
	{ Key::BackToCheckpoint, sf::Keyboard::B },
	{ Key::ToggleAutotarget, sf::Keyboard::P },
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
	{ Key::Move_Left, sf::Keyboard::Left },
	{ Key::Move_Right, sf::Keyboard::Right },
	{ Key::Move_Up, sf::Keyboard::Up },
	{ Key::Move_Down, sf::Keyboard::Down },
	{ Key::Jump, sf::Keyboard::KeyCount },
	{ Key::Chop, sf::Keyboard::KeyCount },
	{ Key::FirstSpell, sf::Keyboard::Numpad1 },
	{ Key::SecondSpell, sf::Keyboard::Numpad2 },
	{ Key::ThirdSpell, sf::Keyboard::Numpad3 },
	{ Key::FourthSpell, sf::Keyboard::Numpad4 },
	{ Key::FifthSpell, sf::Keyboard::Numpad5 },
	{ Key::QuickSlot1, sf::Keyboard::Numpad6 },
	{ Key::QuickSlot2, sf::Keyboard::Numpad7 },
	{ Key::PreviousSpell, sf::Keyboard::KeyCount },
	{ Key::NextSpell, sf::Keyboard::KeyCount },
	{ Key::ToggleTooltips, sf::Keyboard::KeyCount },
	{ Key::SwitchTarget, sf::Keyboard::KeyCount },
	{ Key::Screenshot, sf::Keyboard::KeyCount },
	{ Key::Debug, sf::Keyboard::KeyCount },
	{ Key::BackToCheckpoint, sf::Keyboard::KeyCount },
	{ Key::ToggleAutotarget, sf::Keyboard::KeyCount },
};
