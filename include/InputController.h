#pragma once

#include "global.h"
#include "Enums/Key.h"
#include "GUI/Cursor.h"

class InputController final {
public:
	InputController();
	~InputController();

	// refreshes the keymap
	void update(const sf::Time& frameTime);

	void setWindow(sf::RenderWindow* window, sf::RenderTexture* texture);
	void setCurrentWindowSize(int width, int height);

	// text input
	void startReadingText();
	void stopReadingText();
	const std::string& getReadText() const;
	void cropReadText(int maxLength);
	void readUnicode(sf::Uint32 character);
	void setLastPressedKey(sf::Keyboard::Key key);

	// mouse wheel input
	void setMouseWheelScrollTicks(float deltaTicks);

	bool isKeyActive(Key key) const;
	bool isKeyJustPressed(Key key) const;
	bool isMouseOver(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isLeftClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isRightClicked(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isLeftPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isRightPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isLeftJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;
	bool isRightJustPressed(const sf::FloatRect* boundingBox, bool useDefaultView) const;

	bool isMousePressedLeft() const;
	bool isMousePressedRight() const;
	bool isMouseJustPressedLeft() const;
	bool isMouseJustPressedLeftRaw() const;
	bool isMouseJustPressedRight() const;
	bool isMouseClickedLeft() const;
	bool isMouseClickedRight() const;

	bool isMouseWheelScrolledDown() const;
	bool isMouseWheelScrolledUp() const;

	// helper methods as their combination is often used
	bool isSelected() const;
	bool isScrolledUp() const;
	bool isScrolledDown() const;

	// is the main window focused?
	bool isWindowFocused() const;

	// locks further input actions in this frame.
	void lockAction();
	bool isActionLocked() const;

	// returns mouse position relative to window & the current view
	const sf::Vector2f& getMousePosition() const;
	// returns the mouse position relative to window & the default view
	const sf::Vector2f& getDefaultViewMousePosition() const;

	// returns the sf::keyboard::key that was pressed in the last frame. If none, returns sf::Keyboard::Unknown
	sf::Keyboard::Key getLastPressedKey() const;

	Cursor& getCursor();

private:
	// initializes the keymaps
	void init();

	std::map<Key, bool> m_keyActiveMap;
	std::map<Key, bool> m_keyJustPressedMap;
	sf::RenderWindow* m_mainWindow;
	sf::RenderTexture* m_renderTexture;
	Cursor m_cursor;

	// Mouse keys can't be set in this maps. Their functions are fixed
	const std::map<Key, sf::Keyboard::Key>* m_mainKeyMap;
	const std::map<Key, sf::Keyboard::Key>* m_alternativeKeyMap;

	bool m_isMousePressedLeft = false;
	bool m_isMousePressedRight = false;
	bool m_isMouseJustPressedLeft = false;
	bool m_isMouseJustPressedRight = false;
	bool m_isMouseClickedLeft = false;
	bool m_isMouseClickedRight = false;

	float m_mouseWheelScrollTicks = 0;

	// current window size. Used for mouse position and dependent on resize events
	sf::Vector2i m_windowSize;
	sf::Vector2f m_windowScale;

	// is the mouse inside our view? we only count mouse clicks if it is so!
	bool m_isMouseInsideView;
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

	sf::Keyboard::Key m_lastPressedKey = sf::Keyboard::Unknown;

	// if an input action is done in the current frame, the GO will switch this to true 
	// and the input controller will ignore all further clicks/actions.
	bool m_isActionLocked = false;
};