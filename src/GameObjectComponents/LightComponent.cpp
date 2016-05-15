#include "GameObjectComponents/LightComponent.h"
#include "Screens/Screen.h"

LightComponent::LightComponent(LightData lightData, GameObject* parent) : GameObjectComponent(parent) {
	m_lightObject = new LightObject(lightData);
	m_lightOffset = lightData.center;
	parent->getScreen()->addObject(m_lightObject);
}

LightComponent::~LightComponent() {
	m_lightObject->setDisposed();
}

void LightComponent::setVisible(bool visible) {
	m_lightObject->setVisible(visible);
}

void LightComponent::flipLightOffsetX(bool flipped) {
	m_isLightOffsetFlippedX = flipped;
}

void LightComponent::setPosition(const sf::Vector2f& pos) {
	m_lightObject->setPosition(m_parent->getPosition() + 
		sf::Vector2f(m_isLightOffsetFlippedX ? -m_lightOffset.x + m_parent->getBoundingBox()->width : m_lightOffset.x, m_lightOffset.y));
}