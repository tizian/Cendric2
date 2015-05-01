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
	SpellChop,
	SpellForcefield
};

class InputController
{
public:
	InputController();
	~InputController();
	
	// refreshes the keymap
	void update();
	// initializes the keymaps
	void init();

	void setWindow(sf::RenderWindow* window);
	void setCurrentWindowSize(int width, int height);

	bool isKeyActive(Key key);
	bool isMouseOver(const sf::FloatRect* boundingBox) const;
	bool isRightClicked(const sf::FloatRect* boundingBox) const;
	bool isMouseJustPressedLeft() const;
	bool isMouseJustPressedRight() const;

	// returns mouse position relative to window
	sf::Vector2f getMousePosition() const;

private:
	std::map<Key, bool> m_keyActiveMap;
	sf::RenderWindow* m_mainWindow;

	// TODO: this map has to be loaded from an .ini file later so it can be changed in the game options.
	// Mouse keys can't be set in this map. Their functions are fix.
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