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

	// text input
	void startReadingText();
	void stopReadingText();
	const std::string& getReadText() const;
	void readUnicode(sf::Uint32 character);

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

	// locks further input actions in this frame.
	void lockAction();
	bool isActionLocked() const;

	// returns mouse position relative to window & the current view
	const sf::Vector2f& getMousePosition() const;
	// returns the mouse position relative to window & the default view
	const sf::Vector2f& getDefaultViewMousePosition() const;

private:
	std::map<Key, bool> m_keyActiveMap;
	std::map<Key, bool> m_keyJustPressedMap;
	sf::RenderWindow* m_mainWindow;

	// Mouse keys can't be set in this maps. Their functions are fixed
	const std::map<Key, sf::Keyboard::Key>* m_mainKeyMap;
	const std::map<Key, sf::Keyboard::Key>* m_alternativeKeyMap;

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
	// the mouse position gets calculated once in every frame. This is the mouse position according to the current view
	sf::Vector2f m_mousePosition;
	// the mouse position according to the default view.
	sf::Vector2f m_defaultViewMousePosition;

	bool m_isReadText = false;
	// the text read by the input controller while isReadText is true
	std::string m_readText;

	bool isKeyPressed(sf::Keyboard::Key key) const;

	// if an input action is done in the current frame, the GO will switch this to true 
	// and the input controller will ignore all further clicks/actions.
	bool m_isActionLocked = false;
};