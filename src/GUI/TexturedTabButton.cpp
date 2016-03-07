#include "GUI/TexturedTabButton.h"

TexturedTabButton::TexturedTabButton(const sf::FloatRect& box) : TabButton(box) {
	m_innerPos = sf::FloatRect(box.left + BORDER_OFFSET, box.top + BORDER_OFFSET, box.width - 2.f * BORDER_OFFSET, box.height - 2.f * BORDER_OFFSET);

	m_texturedLayer = sf::RectangleShape();
	m_texturedLayer.setPosition(sf::Vector2f(m_innerPos.left, m_innerPos.top));
	m_texturedLayer.setSize(sf::Vector2f(m_innerPos.width, m_innerPos.height));
}

void TexturedTabButton::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_background);
	renderTarget.draw(m_texturedLayer);
	renderTarget.draw(m_border);
	renderTarget.draw(m_text);
}

void TexturedTabButton::setPosition(const sf::Vector2f& pos) {
	TabButton::setPosition(pos);

	sf::Vector2f newInnerPos = getPosition() + sf::Vector2f(
		2 * BORDER_OFFSET + 0.5f * (getBoundingBox()->width - 2 * BORDER_OFFSET - m_innerPos.width),
		2 * BORDER_OFFSET + 0.5f * (getBoundingBox()->height - 2 * BORDER_OFFSET - m_innerPos.height));
	m_innerPos.left = newInnerPos.x;
	m_innerPos.top = newInnerPos.y;
	m_texturedLayer.setPosition(newInnerPos);
}

void TexturedTabButton::setTexture(const sf::Texture* tex, const sf::IntRect& textureRect) {
	m_texturedLayer.setTexture(tex);
	m_texturedLayer.setTextureRect(textureRect);

	m_innerPos.height = std::min(getBoundingBox()->height, static_cast<float>(textureRect.height));
	m_innerPos.width = textureRect.width * (m_innerPos.height / static_cast<float>(textureRect.height));
	m_texturedLayer.setSize(sf::Vector2f(m_innerPos.width, m_innerPos.height));

	setPosition(getPosition());
}

void TexturedTabButton::setTextureColor(const sf::Color& color) {
	m_texturedLayer.setFillColor(color);
}
