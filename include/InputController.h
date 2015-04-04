#pragma once

#include <map>

#include "global.h"

enum class Key
{
	Left,
	Right, 
	Jump, 
	Fire, 
	SpellFire, 
	SpellIce
};

class InputController
{
public:
	InputController();
	~InputController();

	bool isKeyActive(const Key key);

	// \brief refreshes the keymap
	void update();

	// \brief initializes the keymaps
	void init();

private:
	std::map<Key, bool> m_keyActiveMap;

	// TODO: this map has to be loaded from an .ini file later so it can be changed in the game options.
	// Another problem: Mouse keys can't be set in this map yet.
	std::map<Key, sf::Keyboard::Key> m_keyMap;
};