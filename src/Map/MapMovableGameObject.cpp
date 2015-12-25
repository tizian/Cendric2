#include "Map/MapMovableGameObject.h"

MapMovableGameObject::MapMovableGameObject(Map* map) : MovableGameObject() {
	m_map = map;
}

MapMovableGameObject::~MapMovableGameObject() {
}

void MapMovableGameObject::updateAnimation(const sf::Time& frameTime) {
	// calculate new game state and set animation.
	GameObjectState newState = m_state;
	// check if char walks up
	if (getVelocity().y < 0.0f && (std::abs(getVelocity().x) < std::abs(getVelocity().y))) {
		newState = GameObjectState::Walking_up;
	}
	else if (getVelocity().y >= 0.0f && (std::abs(getVelocity().x) < std::abs(getVelocity().y))) {
		newState = GameObjectState::Walking_down;
	}
	else if (getVelocity().x < 0.0f && (std::abs(getVelocity().x) > std::abs(getVelocity().y))) {
		newState = GameObjectState::Walking_left;
	}
	else if (getVelocity().x >= 0.0f && (std::abs(getVelocity().x) > std::abs(getVelocity().y))) {
		newState = GameObjectState::Walking_right;
	}

	// check if char is standing still
	if (getVelocity().x == 0.0f && getVelocity().y == 0.0f) {
		if (m_state == GameObjectState::Walking_down) {
			newState = GameObjectState::Idle_down;
		}
		else if (m_state == GameObjectState::Walking_up) {
			newState = GameObjectState::Idle_up;
		}
		else if (m_state == GameObjectState::Walking_right) {
			newState = GameObjectState::Idle_right;
		}
		else if (m_state == GameObjectState::Walking_left) {
			newState = GameObjectState::Idle_left;
		}
	}

	// only update animation if we need to
	if (m_state != newState) {
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), false);
	}
}

Map* MapMovableGameObject::getMap() const {
	return m_map;
}