#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/Language.h"
#include "Enums/Key.h"

// describes the configuration that can be modified by the user.
// it is load every time the game starts and stored in the resource manager.
struct ConfigurationData
{
	Language language;
	bool isSoundOn;
	// in percent
	int volume;
	// Framerate. Values < 30 give interesting results and should not be selected. Values > 80 do not make sense.
	int maxFrameRate;
	std::map < Key, sf::Keyboard::Key > mainKeyMap;
	// TODO: the alternative key map can't be seen, saved or changed by the user yet.
	std::map < Key, sf::Keyboard::Key > alternativeKeyMap;
	bool isQuickcast;
};

const struct ConfigurationData DEFAULT_CONFIGURATION =
{
	Language::Lang_EN,
	true,
	100,
	80,
	std::map < Key, sf::Keyboard::Key >(
	{
		{ Key::Escape, sf::Keyboard::Escape },
		{ Key::Quickload, sf::Keyboard::F9 },
		{ Key::Quicksave, sf::Keyboard::F5 },
		{ Key::Inventory, sf::Keyboard::I },
		{ Key::CharacterInfo, sf::Keyboard::C },
		{ Key::Journal, sf::Keyboard::J },
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
		{ Key::FifthSpell, sf::Keyboard::Num5 }
	}),
	std::map < Key, sf::Keyboard::Key >(
		{
			// assuming that it will never be "KeyCount" as input.
			// the other option was sf::Keyboard::Unkonwn, but those'd be are all unhandled keys...
			{ Key::Escape, sf::Keyboard::KeyCount },
			{ Key::Quickload, sf::Keyboard::KeyCount },
			{ Key::Quicksave, sf::Keyboard::KeyCount },
			{ Key::Inventory, sf::Keyboard::KeyCount },
			{ Key::CharacterInfo, sf::Keyboard::KeyCount },
			{ Key::Journal, sf::Keyboard::KeyCount },
			{ Key::Interact, sf::Keyboard::KeyCount },
			{ Key::Confirm, sf::Keyboard::KeyCount },
			{ Key::Left, sf::Keyboard::Left },
			{ Key::Right, sf::Keyboard::Right },
			{ Key::Up, sf::Keyboard::Up},
			{ Key::Down, sf::Keyboard::Down },
			{ Key::Jump, sf::Keyboard::KeyCount },
			{ Key::Chop, sf::Keyboard::KeyCount },
			{ Key::FirstSpell, sf::Keyboard::Numpad1 },
			{ Key::SecondSpell, sf::Keyboard::Numpad2 },
			{ Key::ThirdSpell, sf::Keyboard::Numpad3 },
			{ Key::FourthSpell, sf::Keyboard::Numpad4 },
			{ Key::FifthSpell, sf::Keyboard::Numpad5 }
		}),
		true,
};