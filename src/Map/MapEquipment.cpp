#include "Map/MapEquipment.h"
#include "Map/MapMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"

MapEquipment::MapEquipment(MapMainCharacter* mainChar) : AnimatedGameObject() {
	m_mainChar = mainChar; 
	m_screen = mainChar->getScreen();
}

void MapEquipment::update(const sf::Time& frameTime) {
	if (!m_hasTexture) {
		setPosition(m_mainChar->getPosition());
		return;
	}

	AnimatedGameObject::update(frameTime);
	GameObjectState newState = m_mainChar->getGameObjectState();

	if (m_state != newState) {
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), false);
	}
	
	setPosition(m_mainChar->getPosition());
}

void MapEquipment::setLightComponent(const LightData& data) {
	m_lightComponent = new LightComponent(data, this);
	addComponent(m_lightComponent);
}

void MapEquipment::setHasTexture() {
	m_hasTexture = true;
}

GameObjectType MapEquipment::getConfiguredType() const {
	return GameObjectType::_Equipment;
}
