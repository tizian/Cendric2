#include "Map/MapEquipment.h"
#include "Map/MapMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"

MapEquipment::MapEquipment(MapMainCharacter* mainChar) : AnimatedGameObject() {
	m_mainChar = mainChar; 
	m_screen = mainChar->getScreen();
}

void MapEquipment::update(const sf::Time& frameTime) {
	GameObjectState newState = m_mainChar->getState();

	if (m_state != newState) {
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), false);
	}
	
	sf::Vector2f newPosition;
	
	setPosition(newPosition);
	AnimatedGameObject::update(frameTime);
}

void MapEquipment::setTexturePath(const std::string& texturePath) {
	m_texturePath = texturePath;
}

GameObjectType MapEquipment::getConfiguredType() const {
	return GameObjectType::_Equipment;
}
