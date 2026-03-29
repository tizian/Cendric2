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
	{ Key::Escape, sf::Keyboard::Key::Escape },
	{ Key::Quickload, sf::Keyboard::Key::F9 },
	{ Key::Quicksave, sf::Keyboard::Key::F5 },
	{ Key::Inventory, sf::Keyboard::Key::I },
	{ Key::CharacterInfo, sf::Keyboard::Key::C },
	{ Key::Map, sf::Keyboard::Key::M },
	{ Key::Journal, sf::Keyboard::Key::J },
	{ Key::Spellbook, sf::Keyboard::Key::K },
	{ Key::Interact, sf::Keyboard::Key::E },
	{ Key::Confirm, sf::Keyboard::Key::Enter },
	{ Key::Move_Left, sf::Keyboard::Key::A },
	{ Key::Move_Right, sf::Keyboard::Key::D },
	{ Key::Move_Up, sf::Keyboard::Key::W },
	{ Key::Move_Down, sf::Keyboard::Key::S },
	{ Key::Jump, sf::Keyboard::Key::Space },
	{ Key::Chop, sf::Keyboard::Key::F },
	{ Key::FirstSpell, sf::Keyboard::Key::Num1 },
	{ Key::SecondSpell, sf::Keyboard::Key::Num2 },
	{ Key::ThirdSpell, sf::Keyboard::Key::Num3 },
	{ Key::FourthSpell, sf::Keyboard::Key::Num4 },
	{ Key::FifthSpell, sf::Keyboard::Key::Num5 },
	{ Key::QuickSlot1, sf::Keyboard::Key::Num6 },
	{ Key::QuickSlot2, sf::Keyboard::Key::Num7 },
	{ Key::PreviousSpell, sf::Keyboard::Key::R },
	{ Key::NextSpell, sf::Keyboard::Key::T },
	{ Key::ToggleTooltips, sf::Keyboard::Key::LAlt },
	{ Key::SwitchTarget, sf::Keyboard::Key::Tab },
	{ Key::Screenshot, sf::Keyboard::Key::F10 },
	{ Key::Debug, sf::Keyboard::Key::F11 },
	{ Key::BackToCheckpoint, sf::Keyboard::Key::B },
	{ Key::ToggleAutotarget, sf::Keyboard::Key::P },
};

const std::map<Key, sf::Keyboard::Key> ConfigurationData::ALTERNATIVE_KEYMAP =
{
	// sf::Keyboard::KeyCount means "not set"
	{ Key::Escape, sf::Keyboard::Key::Unknown },
	{ Key::Quickload, sf::Keyboard::Key::Unknown },
	{ Key::Quicksave, sf::Keyboard::Key::Unknown },
	{ Key::Inventory, sf::Keyboard::Key::Unknown },
	{ Key::CharacterInfo, sf::Keyboard::Key::Unknown },
	{ Key::Map, sf::Keyboard::Key::Unknown },
	{ Key::Journal, sf::Keyboard::Key::Unknown },
	{ Key::Spellbook, sf::Keyboard::Key::Unknown },
	{ Key::Interact, sf::Keyboard::Key::Unknown },
	{ Key::Confirm, sf::Keyboard::Key::Unknown },
	{ Key::Move_Left, sf::Keyboard::Key::Left },
	{ Key::Move_Right, sf::Keyboard::Key::Right },
	{ Key::Move_Up, sf::Keyboard::Key::Up },
	{ Key::Move_Down, sf::Keyboard::Key::Down },
	{ Key::Jump, sf::Keyboard::Key::Unknown },
	{ Key::Chop, sf::Keyboard::Key::Unknown },
	{ Key::FirstSpell, sf::Keyboard::Key::Numpad1 },
	{ Key::SecondSpell, sf::Keyboard::Key::Numpad2 },
	{ Key::ThirdSpell, sf::Keyboard::Key::Numpad3 },
	{ Key::FourthSpell, sf::Keyboard::Key::Numpad4 },
	{ Key::FifthSpell, sf::Keyboard::Key::Numpad5 },
	{ Key::QuickSlot1, sf::Keyboard::Key::Numpad6 },
	{ Key::QuickSlot2, sf::Keyboard::Key::Numpad7 },
	{ Key::PreviousSpell, sf::Keyboard::Key::Unknown },
	{ Key::NextSpell, sf::Keyboard::Key::Unknown },
	{ Key::ToggleTooltips, sf::Keyboard::Key::Unknown },
	{ Key::SwitchTarget, sf::Keyboard::Key::Unknown },
	{ Key::Screenshot, sf::Keyboard::Key::Unknown },
	{ Key::Debug, sf::Keyboard::Key::Unknown },
	{ Key::BackToCheckpoint, sf::Keyboard::Key::Unknown },
	{ Key::ToggleAutotarget, sf::Keyboard::Key::Unknown },
};