#include "GUI/TexturedTabButton.h"

TexturedTabButton::TexturedTabButton(const sf::FloatRect& box) : TabButton(box) {
	sf::FloatRect innerBox = sf::FloatRect(box.left + BORDER_OFFSET, box.top + BORDER_OFFSET, box.width - 2.f * BORDER_OFFSET, box.height - 2.f * BORDER_OFFSET);

	m_texturedLayer = sf::RectangleShape();
	m_texturedLayer.setPosition(sf::Vector2f(innerBox.left, innerBox.top));
	m_texturedLayer.setSize(sf::Vector2f(innerBox.width, innerBox.height));
}

void TexturedTabButton::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_background);
	renderTarget.draw(m_texturedLayer);
	renderTarget.draw(m_border);
	renderTarget.draw(m_text);
}

void TexturedTabButton::setPosition(const sf::Vector2f& pos) {
	TabButton::setPosition(pos);
	sf::Vector2f innerPos(pos.x + BORDER_OFFSET, pos.y + BORDER_OFFSET);
	m_texturedLayer.setPosition(innerPos);
}

void TexturedTabButton::setTexture(const sf::Texture* tex, const sf::IntRect& textureRect) {
	m_texturedLayer.setTexture(tex);
	m_texturedLayer.setTextureRect(textureRect);
}

void TexturedTabButton::setTextureColor(const sf::Color& color) {
	m_texturedLayer.setFillColor(color);
}
