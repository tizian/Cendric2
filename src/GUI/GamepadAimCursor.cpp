#include "GUI/GamepadAimCursor.h"
#include "GlobalResource.h"
#include "ResourceManager.h"
#include "Controller/InputController.h"
#include "Level/LevelMainCharacter.h"

const float GamepadAimCursor::AIM_DISTANCE = 100.f;

GamepadAimCursor::GamepadAimCursor(LevelMainCharacter* character) {
	m_cursorSprite.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_CURSOR));
	g_resourceManager->getTexture(GlobalResource::TEX_GUI_CURSOR)->setSmooth(true);
	m_cursorSprite.setTextureRect(sf::IntRect(100, 0, 50, 40));
	m_cursorSprite.setOrigin(sf::Vector2f(24.f, 22.f));
	m_mainChar = character;
	m_isAlwaysUpdate = true;
	m_isVisible = true;
	m_isActive = false;
	m_currentAimOffset = sf::Vector2f(AIM_DISTANCE, 0);

	character->getScreen()->addObject(this);
}

void GamepadAimCursor::update(const sf::Time& frameTime) {
	m_isActive = g_inputController->isJoystickConnected();
	if (!m_isVisible || !m_isActive) return;

	auto axis = g_inputController->getRightJoystickAxis();
	if (norm(axis) > 20) {
		m_currentAimOffset = AIM_DISTANCE * normalized(axis);
		m_cursorSprite.setRotation(radToDeg(atan2(m_currentAimOffset.y, m_currentAimOffset.x)) + 90);
	}

	m_currentPosition = m_mainChar->getPosition() + sf::Vector2f(m_mainChar->getBoundingBox()->width * 0.5f, 0.f) + m_currentAimOffset;
	m_cursorSprite.setPosition(m_currentPosition);
}

void GamepadAimCursor::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible || !m_isActive) return;
	target.draw(m_cursorSprite);
}

void GamepadAimCursor::setVisible(bool visible) {
	m_isVisible = visible;
}

const sf::Vector2f& GamepadAimCursor::getCurrentPosition() const {
	return m_currentPosition;
}