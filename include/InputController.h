#include "stdafx.h"

enum Key
{
	LEFT, 
	RIGHT, 
	JUMP, 
	FIRE, 
	SPELL_FIRE, 
	SPELL_ICE
};

class InputController
{
public:
	InputController();
	~InputController();

	// getters
	unordered_map<Key, bool> getKeyActiveMap();
	unordered_map<Key, sf::Keyboard::Key> getKeyMap();

	// \brief refreshes the keymap
	void update();

	// \brief initializes the keymaps
	void init();

private:
	unordered_map<Key, bool> m_keyActiveMap;

	// TODO: this map has to be loaded from an .ini file later so it can be changed in the game options.
	// Another problem: Mouse keys can't be set in this map yet.
	unordered_map<Key, sf::Keyboard::Key> m_keyMap;
};