#include "Level/LevelEquipment.h"
#include "Level/LevelMainCharacter.h"

LevelEquipment::~LevelEquipment() {
	g_resourceManager->deleteResource(m_texturePath);
}

void LevelEquipment::calculatePositionAccordingToMainChar(sf::Vector2f& position) const {
	sf::Vector2f mainCharPosition(m_mainChar->getPosition().x + (m_mainChar->getBoundingBox()->width / 2), m_mainChar->getPosition().y);
	sf::Vector2f offset(-60.f, -20.f);
	if (!m_mainChar->getIsFacingRight())
		offset.x = -offset.x - getBoundingBox()->width;
	if (m_mainChar->getIsUpsideDown())
		offset.y = m_mainChar->getBoundingBox()->height - offset.y - getBoundingBox()->height;

	position.x = (mainCharPosition + offset).x;
	position.y = (mainCharPosition + offset).y;
}

void LevelEquipment::setPosition(const sf::Vector2f& position) {
	AnimatedGameObject::setPosition(position);
	sf::Vector2f mainCharPosition(m_mainChar->getPosition().x + (m_mainChar->getBoundingBox()->width / 2), m_mainChar->getPosition().y);
	if (m_lightObject != nullptr) {
		sf::Vector2f lightPosition = mainCharPosition + m_lightObjectOffset;
		if (!m_mainChar->getIsFacingRight()) {
			lightPosition.x -= 2 * m_lightObjectOffset.x;
		}
		
		m_lightObject->setPosition(lightPosition);
	}
}

void LevelEquipment::update(const sf::Time& frameTime) {
	AnimatedGameObject::update(frameTime);
	GameObjectState newState = m_mainChar->getState();
	if (newState == GameObjectState::Dead) {
		setDisposed();
		return;
	}
	bool newFacingRight = m_mainChar->getIsFacingRight();
	if (m_state != newState || newFacingRight != m_isFacingRight) {
		m_state = newState;
		m_isFacingRight = newFacingRight;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
	if (m_mainChar->getIsUpsideDown() != m_animatedSprite.isFlippedY()) {
		m_animatedSprite.setFlippedY(m_mainChar->getIsUpsideDown());
	}

	if (m_isCopyingMainCharColor)
		setSpriteColor(m_mainChar->getCurrentSpriteColor(), sf::milliseconds(1));

	sf::Vector2f newPosition;
	calculatePositionAccordingToMainChar(newPosition);
	setPosition(newPosition);
}

void LevelEquipment::loadEquipment(LevelMainCharacter* mainChar) {
	m_mainChar = mainChar;

	sf::Vector2f position;
	calculatePositionAccordingToMainChar(position);
	setPosition(position);
}

void LevelEquipment::setTexturePath(const std::string& texturePath) {
	m_texturePath = texturePath;
}

void LevelEquipment::setLightObject(LightObject* lightObject) {
	m_lightObject = lightObject;
	m_lightObjectOffset = m_lightObject->getCenter();
}

GameObjectType LevelEquipment::getConfiguredType() const {
	return GameObjectType::_LevelEquipment;
}

void LevelEquipment::setDisposed() {
	AnimatedGameObject::setDisposed();
	if (m_lightObject != nullptr)
		m_lightObject->setDisposed();
}

void LevelEquipment::setScreen(Screen* screen) {
	AnimatedGameObject::setScreen(screen);
	if (m_lightObject != nullptr)
		screen->addObject(m_lightObject);
}

void LevelEquipment::setCopyingMainCharColor(bool value) {
	m_isCopyingMainCharColor = value;
}
