#include "Map/MapEquipment.h"
#include "Map/MapMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"

MapEquipment::MapEquipment(MapMainCharacter* mainChar) : AnimatedGameObject() {
	m_mainChar = mainChar; 
	m_screen = mainChar->getScreen();
}

void MapEquipment::update(const sf::Time& frameTime) {
	AnimatedGameObject::update(frameTime);
	GameObjectState newState = m_mainChar->getGameObjectState();

	if (m_state != newState) {
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), false);
	}
	
	setPosition(m_mainChar->getPosition());
}

GameObjectType MapEquipment::getConfiguredType() const {
	return GameObjectType::_Equipment;
}
