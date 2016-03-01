#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/Language.h"
#include "Enums/Key.h"

// describes the configuration that can be modified by the user.
// it is loaded every time the game starts and stored in the resource manager.
struct ConfigurationData {
	Language language;
	bool isSoundOn;
	// in percent
	int volumeSound;
	// in percent
	int volumeMusic;
	bool isVSyncEnabled;
	std::map < Key, sf::Keyboard::Key > mainKeyMap;
	// important: the alternative key map can't be seen, saved or changed by the user yet.
	std::map < Key, sf::Keyboard::Key > alternativeKeyMap;
	bool isQuickcast;
	bool isFullscreen;
	bool isSmoothing;
	bool isDebugMode;
	bool isDebugRendering;
	bool isWindowReload;
	bool isDisplayHints;
};

const std::map < Key, sf::Keyboard::Key > DEFAULT_KEYMAP =
std::map < Key, sf::Keyboard::Key >(
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
	{ Key::ToggleTooltips, sf::Keyboard::Tab }
});

const struct ConfigurationData DEFAULT_CONFIGURATION =
{
	Language::Lang_EN,
	true,
	100,
	100,
	true,
	DEFAULT_KEYMAP,
	std::map < Key, sf::Keyboard::Key >(
	{
		// assuming that it will never be "KeyCount" as input.
		// the other option was sf::Keyboard::Unknown, but those'd be are all unhandled keys...
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
		{ Key::FirstSpell, sf::Keyboard::Numpad1 },
		{ Key::SecondSpell, sf::Keyboard::Numpad2 },
		{ Key::ThirdSpell, sf::Keyboard::Numpad3 },
		{ Key::FourthSpell, sf::Keyboard::Numpad4 },
		{ Key::FifthSpell, sf::Keyboard::Numpad5 },
		{ Key::QuickSlot1, sf::Keyboard::Numpad6 },
		{ Key::QuickSlot2, sf::Keyboard::Numpad7 },
		{ Key::ToggleTooltips, sf::Keyboard::KeyCount }
	}),
	true,
	false,
	true,
	false,
	false,
	false,
	true,
};