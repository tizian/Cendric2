#pragma once

#include <map>

#include "global.h"
#include "Enums/Key.h"

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
	bool isKeyJustPressed(Key key);
	bool isMouseOver(const sf::FloatRect* boundingBox) const;
	bool isLeftClicked(const sf::FloatRect* boundingBox) const;
	bool isRightClicked(const sf::FloatRect* boundingBox) const;
	bool isLeftPressed(const sf::FloatRect* boundingBox) const;
	bool isRightPressed(const sf::FloatRect* boundingBox) const;
	bool isLeftJustPressed(const sf::FloatRect* boundingBox) const;
	bool isRightJustPressed(const sf::FloatRect* boundingBox) const;
	
	bool isMousePressedLeft() const;
	bool isMousePressedRight() const;
	bool isMouseJustPressedLeft() const;
	bool isMouseJustPressedRight() const;
	bool isMouseClickedLeft() const;
	bool isMouseClickedRight() const;

	// returns mouse position relative to window
	const sf::Vector2f& getMousePosition() const;

private:
	std::map<Key, bool> m_keyActiveMap;
	std::map<Key, bool> m_keyJustPressedMap;
	sf::RenderWindow* m_mainWindow;

	// TODO: this map has to be loaded from an .ini file later so it can be changed in the game options.
	// Mouse keys can't be set in this map. Their functions are fix.
	std::map<Key, sf::Keyboard::Key> m_keyMap;

	bool m_isMousePressedLeft = false;
	bool m_isMousePressedRight = false;
	bool m_isMouseJustPressedLeft = false;
	bool m_isMouseJustPressedRight = false;
	bool m_isMouseClickedLeft = false;
	bool m_isMouseClickedRight = false;

	// current window size. Used for mouse position and dependent on resize events
	sf::Vector2i m_windowSize;
	
	// is the window currently focused? we only count input events if it is so!
	bool m_isWindowFocused;
	// the mouse position gets calculated in every frame.
	sf::Vector2f m_mousePosition;
};