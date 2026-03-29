#include "GUI/ScrollHelper.h"
#include "GUI/ScrollBar.h"

ScrollHelper::ScrollHelper(const sf::FloatRect& boundingBox)
	: lastOffset(0.f), nextOffset(0.f), m_boundingBox(boundingBox) {
	texture.resize({WINDOW_WIDTH, WINDOW_HEIGHT});
	texture.clear(sf::Color(0, 0, 0, 0));
	m_sprite->setTextureRect(static_cast<sf::IntRect>(m_boundingBox));
	m_sprite->setPosition({boundingBox.position.x, boundingBox.position.y});
}

void ScrollHelper::setPosition(const sf::Vector2f& position) {
	m_boundingBox.position.x = position.x;
	m_boundingBox.position.y = position.y;
	m_sprite->setPosition(position);
	m_sprite->setTextureRect(static_cast<sf::IntRect>(m_boundingBox));
}

void ScrollHelper::render(sf::RenderTarget& target) {
	texture.display();
	m_sprite->setTexture(texture.getTexture());
	target.draw(*m_sprite);
	texture.clear(sf::Color(0, 0, 0, 0));
}
