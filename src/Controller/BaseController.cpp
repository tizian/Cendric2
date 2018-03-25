#include "ResourceManager.h"
#include "Controller/BaseController.h"

BaseController::BaseController() {
}

void BaseController::lockAction() {
	m_isActionLocked = true;
}

bool BaseController::isActionLocked() const {
	return m_isActionLocked;
}

void BaseController::update(const sf::Time& frameTime) {
	bool previousFocus = m_isWindowFocused;
	m_isWindowFocused = m_mainWindow->hasFocus();
	m_isActionLocked = false;

	if (previousFocus != m_isWindowFocused) {
		g_resourceManager->notifyVolumeChanged();
	}

	m_cursor.update(frameTime);
}

void BaseController::setWindow(sf::RenderWindow* window, sf::RenderTexture* texture, const sf::Vector2f& spriteScale) {
	m_mainWindow = window;
	m_renderTexture = texture;
	m_spriteScale = spriteScale;
	m_isWindowFocused = m_mainWindow->hasFocus();
	setCurrentWindowSize(m_mainWindow->getSize().x, m_mainWindow->getSize().y);
}

void BaseController::setCurrentWindowSize(int width, int height) {
	m_windowScale.x = (width / (static_cast<float>(WINDOW_WIDTH))) / m_spriteScale.x;
	m_windowScale.y = (height / (static_cast<float>(WINDOW_HEIGHT))) / m_spriteScale.y;
	m_cursor.setScale(m_spriteScale);
}

bool BaseController::isWindowFocused() const {
	return m_isWindowFocused;
}

Cursor& BaseController::getCursor() {
	return m_cursor;
}

