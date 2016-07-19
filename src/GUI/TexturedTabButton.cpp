#include "GUI/TexturedTabButton.h"

TexturedTabButton::TexturedTabButton(const sf::FloatRect& box) : TabButton(box) {}

void TexturedTabButton::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_background);
	renderTarget.draw(m_texturedLayer);
	renderTarget.draw(m_border);
	renderTarget.draw(m_text);
}

void TexturedTabButton::setTexture(const sf::Texture* tex, const sf::IntRect& textureRect) {
	m_texturedLayer.setTexture(tex);
	m_texturedLayer.setTextureRect(textureRect);

	const sf::FloatRect* bbox = getBoundingBox();

	float height = std::min(bbox->height, static_cast<float>(textureRect.height));
	float width = textureRect.width * (height / static_cast<float>(textureRect.height));
	float x = getBoundingBox()->left + 0.5f * (getBoundingBox()->width - width);
	float y = getBoundingBox()->top + 0.5f * (getBoundingBox()->height - height);

	m_texturedLayer.setPosition(sf::Vector2f(x, y));
	m_texturedLayer.setSize(sf::Vector2f(width, height));
}

void TexturedTabButton::setTextureColor(const sf::Color& color) {
	m_texturedLayer.setFillColor(color);
}
