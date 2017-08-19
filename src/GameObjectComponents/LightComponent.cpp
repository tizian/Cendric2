#include "GameObjectComponents/LightComponent.h"
#include "Screens/Screen.h"

LightComponent::LightComponent(LightData lightData, GameObject* parent) : GameObjectComponent(parent) {
	m_lightObject = new LightObject(lightData);
	m_offset = lightData.center;
	parent->getScreen()->addObject(m_lightObject);
}

LightComponent::~LightComponent() {
	m_lightObject->setDisposed();
}

void LightComponent::setVisible(bool visible) {
	m_lightObject->setVisible(visible);
}

void LightComponent::flipOffsetX(bool flipped) {
	m_isOffsetFlippedX = flipped;
}

void LightComponent::flipOffsetY(bool flipped) {
	m_isOffsetFlippedY = flipped;
}

void LightComponent::setPosition(const sf::Vector2f& pos) {
	m_lightObject->setPosition(m_parent->getPosition() + 
		sf::Vector2f(
			m_isOffsetFlippedX ? -m_offset.x + m_parent->getBoundingBox()->width : m_offset.x,
			m_isOffsetFlippedY ? -m_offset.y + m_parent->getBoundingBox()->height : m_offset.y));
}

void LightComponent::setBrightness(float brightness) {
	m_lightObject->setBrightness(brightness);
}