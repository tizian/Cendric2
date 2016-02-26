#include "ScreenOverlay.h"

ScreenOverlay::ScreenOverlay(ResourceID texture, const sf::Time& activeTime) : GameObject() {
	m_sprite.setTexture(*g_resourceManager->getTexture(texture));
	m_sprite.setPosition(sf::Vector2f(0.f, 0.f));
	m_activeTime = activeTime;
	m_isAlwaysUpdate = true;
	m_isInputInDefaultView = true;

	setBoundingBox(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
}

ScreenOverlay::~ScreenOverlay() {
}

void ScreenOverlay::update(const sf::Time& frameTime) {
	if (m_activeTime > sf::Time::Zero) {
		GameObject::updateTime(m_activeTime, frameTime);
		if (m_activeTime == sf::Time::Zero) {
			setDisposed();
		}
	}
}

void ScreenOverlay::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_sprite);
}

GameObjectType ScreenOverlay::getConfiguredType() const {
	return GameObjectType::_ScreenOverlay;
}