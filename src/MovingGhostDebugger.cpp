#include "MovingGhostDebugger.h"

MovingGhostDebugger::MovingGhostDebugger() {
	m_renderTime = sf::seconds(3.f);
}

MovingGhostDebugger::~MovingGhostDebugger() {}

void MovingGhostDebugger::addDebugBoundingBox(const sf::FloatRect& boundingBox) {
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
	shape.setPosition(sf::Vector2f(boundingBox.left, boundingBox.top));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(2.f);
	setBoundingBox(boundingBox);
	setPosition(shape.getPosition());
	m_debugBoxes.push_back(shape);
}

void MovingGhostDebugger::update(const sf::Time& frameTime) {
	GameObject::updateTime(m_renderTime, frameTime);
	if (m_renderTime == sf::Time::Zero) {
		setDisposed();
	}
}

void MovingGhostDebugger::render(sf::RenderTarget& target) {
	for (auto& box : m_debugBoxes) {
		target.draw(box);
	}
}

GameObjectType MovingGhostDebugger::getConfiguredType() const {
	return GameObjectType::_Overlay;
}
