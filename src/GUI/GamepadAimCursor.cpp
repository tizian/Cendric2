#include "GUI/GamepadAimCursor.h"
#include "GlobalResource.h"
#include "ResourceManager.h"
#include "Controller/InputController.h"
#include "Level/LevelMainCharacter.h"

const float GamepadAimCursor::AIM_DISTANCE = 100.f;
const sf::Time GamepadAimCursor::IN_USE_TIME = sf::seconds(3.f);

GamepadAimCursor::GamepadAimCursor(LevelMainCharacter* character) {
	m_cursorSprite.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_CURSOR));
	g_resourceManager->getTexture(GlobalResource::TEX_GUI_CURSOR)->setSmooth(true);
	m_cursorSprite.setTextureRect(sf::IntRect(100, 0, 50, 40));
	m_cursorSprite.setOrigin(sf::Vector2f(24.f, 22.f));
	m_mainChar = character;
	m_isAlwaysUpdate = true;
	m_isVisible = true;
	m_isActive = false;
	setInUse(!g_resourceManager->getConfiguration().isAutotarget);
	m_currentAimOffset = sf::Vector2f(AIM_DISTANCE, 0);
	m_cursorSprite.setRotation(radToDeg(atan2(m_currentAimOffset.y, m_currentAimOffset.x)) + 90);

	character->getScreen()->addObject(this);
}

void GamepadAimCursor::update(const sf::Time& frameTime) {
	m_isActive = g_inputController->isJoystickConnected();
	if (!m_isVisible || !m_isActive) return;

	auto axis = g_inputController->getRightJoystickAxis();
	if (norm(axis) > 20) {
		m_currentAimOffset = AIM_DISTANCE * normalized(axis);
		m_cursorSprite.setRotation(radToDeg(atan2(m_currentAimOffset.y, m_currentAimOffset.x)) + 90);
		setInUse(true);
	}

	m_currentPosition = m_mainChar->getPosition() + sf::Vector2f(m_mainChar->getBoundingBox()->width * 0.5f, 0.f) + m_currentAimOffset;
	m_cursorSprite.setPosition(m_currentPosition);

	updateInUse(frameTime);
}

void GamepadAimCursor::updateInUse(const sf::Time& frameTime) {
	if (!g_resourceManager->getConfiguration().isAutotarget) {
		if (!m_isInUse) {
			setInUse(true);
		}
		return;
	}

	if (!m_isInUse) {
		return;
	}

	updateTime(m_inUseTime, frameTime);
	if (m_inUseTime == sf::Time::Zero) {
		setInUse(false);
	}
}

void GamepadAimCursor::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isVisible || !m_isActive) return;
	target.draw(m_cursorSprite);
}

void GamepadAimCursor::setVisible(bool visible) {
	m_isVisible = visible;
}

void GamepadAimCursor::setInUse(bool inUse) {
	m_isInUse = inUse;

	auto unusedColor = sf::Color::White;
	unusedColor.a = 100;
	m_cursorSprite.setColor(inUse ? sf::Color::White : unusedColor);

	if (m_isInUse) {
		m_inUseTime = IN_USE_TIME;
	}
}

bool GamepadAimCursor::isUseAutotarget() const {
	return !m_isInUse && m_isActive && m_isVisible;
}

const sf::Vector2f& GamepadAimCursor::getCurrentPosition() const {
	return m_currentPosition;
}