#pragma once

#include <map>

#include "global.h"

enum class Key
{
	Escape,
	Left,
	Right, 
	Up,
	Down,
	Jump, 
	Fire, 
	SpellFire, 
	SpellIce,
	SpellChop
};

class InputController
{
public:
	InputController();
	~InputController();

	bool isKeyActive(const Key key);

	bool isMouseJustPressedLeft();
	bool isMouseJustPressedRight();

	// refreshes the keymap
	void update();

	// initializes the keymaps
	void init();

	void setWindow(sf::RenderWindow* window);
	void setCurrentWindowSize(int width, int height);

	// returns mouse position relative to window
	sf::Vector2f getMousePosition();

private:
	std::map<Key, bool> m_keyActiveMap;
	sf::RenderWindow* m_mainWindow;

	// TODO: this map has to be loaded from an .ini file later so it can be changed in the game options.
	// Another problem: Mouse keys can't be set in this map yet.
	std::map<Key, sf::Keyboard::Key> m_keyMap;

	bool m_isMouseReleasedLeft;
	bool m_isMouseReleasedRight;
	bool m_isMouseJustPressedLeft;
	bool m_isMouseJustPressedRight;

	// current window size. Used for mouse position and dependent on resize events
	sf::Vector2i m_windowSize;
	
	// is the window currently focused? we only count input events if it is so!
	bool m_isWindowFocused;
};