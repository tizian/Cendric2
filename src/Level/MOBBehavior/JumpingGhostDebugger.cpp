#include "Level/MOBBehavior/JumpingGhostDebugger.h"

JumpingGhostDebugger::JumpingGhostDebugger() {
	m_renderTime = sf::seconds(1.5f);
}

JumpingGhostDebugger::~JumpingGhostDebugger() {}

void JumpingGhostDebugger::addDebugBoundingBox(const sf::FloatRect& boundingBox) {
	setBoundingBox(boundingBox);
	sf::RectangleShape shape;
	shape.setSize(getSize());
	shape.setPosition(getPosition());
	shape.setFillColor(COLOR_TRANSPARENT);
	shape.setOutlineColor(COLOR_BAD);
	shape.setOutlineThickness(1.f);
	setPosition(shape.getPosition());
	m_debugBoxes.push_back(shape);
}

void JumpingGhostDebugger::setGoodTrajectory() {
	for (auto& box : m_debugBoxes) {
		box.setOutlineColor(COLOR_GOOD);
	}
}

void JumpingGhostDebugger::update(const sf::Time& frameTime) {
	updateTime(m_renderTime, frameTime);
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
	return _Overlay;
}
