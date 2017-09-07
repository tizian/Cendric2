#include "Level/DynamicTiles/MovingParent.h"

MovingParent::MovingParent() {
	m_relativeVelocity.x = 0.f;
	m_relativeVelocity.y = 0.f;
}

MovingParent::~MovingParent() {
}

const sf::Vector2f& MovingParent::getRelativeVelocity() const {
	return m_relativeVelocity;
}