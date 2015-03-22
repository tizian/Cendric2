#include "stdafx.h"

class ResourceLoader
{
public:
	InputController();
	~InputController();

	// getters
	map<Key, bool> getKeyActiveMap();
	map<Key, sf::Keyboard::Key> getKeyMap();

	// \brief refreshes the keymap
	void update();

	// \brief initializes the keymaps
	void init();

private:
	map<Key, bool> m_keyActiveMap;

	// TODO: this map has to be loaded from an .ini file later so it can be changed in the game options.
	// Another problem: Mouse keys can't be set in this map yet.
	map<Key, sf::Keyboard::Key> m_keyMap;
};