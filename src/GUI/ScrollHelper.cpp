#include "GUI/ScrollHelper.h"
#include "GUI/ScrollBar.h"

ScrollHelper::ScrollHelper(const sf::FloatRect &boundingBox)
	: lastOffset(0.f), nextOffset(0.f), m_boundingBox(boundingBox) {
	texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	texture.clear(sf::Color(0, 0, 0, 0));
	m_sprite.setTextureRect(static_cast<sf::IntRect>(m_boundingBox));
	m_sprite.setPosition(boundingBox.left, boundingBox.top);
}

void ScrollHelper::render(sf::RenderTarget& target) {
	texture.display();
	m_sprite.setTexture(texture.getTexture());
	target.draw(m_sprite);
	texture.clear(sf::Color(0, 0, 0, 0));
}
