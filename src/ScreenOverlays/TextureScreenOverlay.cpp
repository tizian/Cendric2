#include "ScreenOverlays/TextureScreenOverlay.h"

TextureScreenOverlay::TextureScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) : ScreenOverlay(activeTime, fadeTime) {
	m_sprite.setPosition(sf::Vector2f(0.f, 0.f));
	m_sprite.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));

	m_background.setPosition(sf::Vector2f(0.f, 0.f));
	m_background.setColor(sf::Color(255, 255, 255, (m_fadeTime > sf::Time::Zero) ? 0 : 255));
}

void TextureScreenOverlay::update(const sf::Time& frameTime) {
	ScreenOverlay::update(frameTime);

	const sf::Color& sc = m_sprite.getColor();
	const sf::Color& bc = m_background.getColor();

	m_sprite.setColor(sf::Color(sc.r, sc.g, sc.b, (sf::Uint8)(m_scale * 255)));
	m_background.setColor(sf::Color(bc.r, bc.g, bc.b, (sf::Uint8)(m_scale * 255)));
}

void TextureScreenOverlay::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_background);
	renderTarget.draw(m_sprite);
	ScreenOverlay::render(renderTarget);
}

void TextureScreenOverlay::setSpriteTexture(const sf::Texture* texture) {
	m_sprite.setTexture(*texture);
}

void TextureScreenOverlay::setSpriteTextureRect(const sf::IntRect& rect) {
	m_sprite.setTextureRect(rect);
}

void TextureScreenOverlay::setSpriteTextureColor(const sf::Color& color) {
	m_sprite.setColor(sf::Color(color.r, color.g, color.b, (sf::Uint8)(m_scale * 255)));
}

void TextureScreenOverlay::setSpritePosition(const sf::Vector2f& position) {
	m_sprite.setPosition(position);
}

void TextureScreenOverlay::setSpriteScale(const sf::Vector2f& factors) {
	m_sprite.setScale(factors);
}

void TextureScreenOverlay::setBackgroundTexture(const sf::Texture* texture) {
	m_background.setTexture(*texture);
}

void TextureScreenOverlay::setBackgroundTextureRect(const sf::IntRect& rect) {
	m_background.setTextureRect(rect);
}

void TextureScreenOverlay::setBackgroundColor(const sf::Color& color) {
	m_background.setColor(sf::Color(color.r, color.g, color.b, (sf::Uint8)(m_scale * 255)));
}

void TextureScreenOverlay::setBackgroundPosition(const sf::Vector2f& position) {
	m_background.setPosition(position);
}

void TextureScreenOverlay::setBackgroundScale(const sf::Vector2f& factors) {
	m_background.setScale(factors);
}