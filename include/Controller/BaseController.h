#pragma once

#include "global.h"
#include "Enums/Key.h"
#include "GUI/Cursor.h"

class BaseController {
public:
	BaseController();
	virtual ~BaseController() {};

	virtual void update(const sf::Time& frameTime);

	void setWindow(sf::RenderWindow* window, sf::RenderTexture* texture, const sf::Vector2f& spriteScale);
	void setCurrentWindowSize(int width, int height);

	// is the main window focused?
	bool isWindowFocused() const;

	// locks further input actions in this frame.
	void lockAction();
	bool isActionLocked() const;

	Cursor& getCursor();

protected:
	sf::RenderWindow* m_mainWindow;
	sf::RenderTexture* m_renderTexture;

	Cursor m_cursor;

	// current window size (only used for fullscreen events). Used for mouse position and dependent on resize events
	sf::Vector2f m_windowScale;
	sf::Vector2f m_spriteScale;

	// is the window currently focused? we only count input events if it is so!
	bool m_isWindowFocused;

	// if an input action is done in the current frame, the GO will switch this to true 
	// and the input controller will ignore all further clicks/actions.
	bool m_isActionLocked = false;
};