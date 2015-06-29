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
	std::map < Key, sf::Keyboard::Key > keyMap;
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
		{ Key::SpellChop, sf::Keyboard::Num1 },
		{ Key::SpellFire, sf::Keyboard::Num2 },
		{ Key::SpellIce, sf::Keyboard::Num3 },
		{ Key::SpellForcefield, sf::Keyboard::Num4 }
	}),
};