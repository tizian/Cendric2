#include "Level/MOBBehavior/JumpingGhostDebugger.h"

JumpingGhostDebugger::JumpingGhostDebugger() {
	m_renderTime = sf::seconds(1.5f);
}

JumpingGhostDebugger::~JumpingGhostDebugger() {}

void JumpingGhostDebugger::addDebugBoundingBox(const sf::FloatRect& boundingBox) {
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
	shape.setPosition(sf::Vector2f(boundingBox.left, boundingBox.top));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	setBoundingBox(boundingBox);
	setPosition(shape.getPosition());
	m_debugBoxes.push_back(shape);
}

void JumpingGhostDebugger::setGoodTrajectory() {
	for (auto& box : m_debugBoxes) {
		box.setOutlineColor(sf::Color::Green);
	}
}

void JumpingGhostDebugger::update(const sf::Time& frameTime) {
	GameObject::updateTime(m_renderTime, frameTime);
	if (m_renderTime == sf::Time::Zero) {
		setDisposed();
	}
}

void JumpingGhostDebugger::render(sf::RenderTarget& target) {
	for (auto& box : m_debugBoxes) {
		target.draw(box);
	}
}

GameObjectType JumpingGhostDebugger::getConfiguredType() const {
	return GameObjectType::_Overlay;
}
