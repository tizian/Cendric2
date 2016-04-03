#include "ScreenOverlays/TextureScreenOverlay.h"

TextureScreenOverlay::TextureScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) : ScreenOverlay(activeTime, fadeTime) {
	
	m_sprite.setPosition(sf::Vector2f(0.f, 0.f));
	m_textureColor = sf::Color(255, 255, 255);
}

void TextureScreenOverlay::update(const sf::Time& frameTime) {
	ScreenOverlay::update(frameTime);
	m_sprite.setColor(sf::Color(
		m_textureColor.r, 
		m_textureColor.g, 
		m_textureColor.b, 
		(sf::Uint8)(m_scale * 255)));
}

void TextureScreenOverlay::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_sprite);
	ScreenOverlay::render(renderTarget);
}

void TextureScreenOverlay::setTexture(ResourceID texture) {
	m_sprite.setTexture(*g_resourceManager->getTexture(texture));
}

void TextureScreenOverlay::setTextureRect(const sf::IntRect& rect) {
	m_sprite.setTextureRect(rect);
}

void TextureScreenOverlay::setTextureColor(const sf::Color& color) {
	m_textureColor = color;
	m_sprite.setColor(color);
}

void TextureScreenOverlay::setSpritePosition(const sf::Vector2f& position) {
	m_sprite.setPosition(position);
}

void TextureScreenOverlay::setSpriteScale(const sf::Vector2f& factors) {
	m_sprite.setScale(factors);
}
