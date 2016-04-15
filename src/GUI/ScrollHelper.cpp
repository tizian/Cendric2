#include "GUI/ScrollHelper.h"
#include "GUI/ScrollBar.h"

ScrollHelper::ScrollHelper(const sf::IntRect &boundingBox) 
	: m_boundingBox(boundingBox) {
	texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	texture.clear(sf::Color(0, 0, 0, 0));
	m_sprite.setTextureRect(m_boundingBox);
}

void ScrollHelper::render(sf::RenderTarget& target) {
	texture.display();
	m_sprite.setTexture(texture.getTexture());
	target.draw(m_sprite);
	texture.clear(sf::Color(0, 0, 0, 0));
}
