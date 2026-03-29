#include "ScreenOverlays/TextureScreenOverlay.h"

TextureScreenOverlay::TextureScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime) : ScreenOverlay(activeTime, fadeTime) {
}

void TextureScreenOverlay::update(const sf::Time& frameTime) {
	ScreenOverlay::update(frameTime);

	if (!m_sprite || !m_background) return;

	const sf::Color& sc = m_sprite->getColor();
	const sf::Color& bc = m_background->getColor();

	m_sprite->setColor(sf::Color(sc.r, sc.g, sc.b, (std::uint8_t)(m_scale * 255)));
	m_background->setColor(sf::Color(bc.r, bc.g, bc.b, (std::uint8_t)(m_scale * 255)));
}

void TextureScreenOverlay::render(sf::RenderTarget& renderTarget) {
	if (m_background) renderTarget.draw(*m_background);
	if (m_sprite) renderTarget.draw(*m_sprite);
	ScreenOverlay::render(renderTarget);
}

void TextureScreenOverlay::setSpriteTexture(const sf::Texture* texture) {
	if (!m_sprite) m_sprite.emplace(*texture);
	else m_sprite->setTexture(*texture);
}

void TextureScreenOverlay::setSpriteTextureRect(const sf::IntRect& rect) {
	if (m_sprite) m_sprite->setTextureRect(rect);
}

void TextureScreenOverlay::setSpriteTextureColor(const sf::Color& color) {
	if (m_sprite) m_sprite->setColor(sf::Color(color.r, color.g, color.b, (std::uint8_t)(m_scale * 255)));
}

void TextureScreenOverlay::setSpritePosition(const sf::Vector2f& position) {
	if (m_sprite) m_sprite->setPosition(position);
}

void TextureScreenOverlay::setSpriteScale(const sf::Vector2f& factors) {
	if (m_sprite) m_sprite->setScale(factors);
}

void TextureScreenOverlay::setBackgroundTexture(const sf::Texture* texture) {
	if (!m_background) m_background.emplace(*texture);
	else m_background->setTexture(*texture);
}

void TextureScreenOverlay::setBackgroundTextureRect(const sf::IntRect& rect) {
	if (m_background) m_background->setTextureRect(rect);
}

void TextureScreenOverlay::setBackgroundColor(const sf::Color& color) {
	if (m_background) m_background->setColor(sf::Color(color.r, color.g, color.b, (std::uint8_t)(m_scale * 255)));
}

void TextureScreenOverlay::setBackgroundPosition(const sf::Vector2f& position) {
	if (m_background) m_background->setPosition(position);
}

void TextureScreenOverlay::setBackgroundScale(const sf::Vector2f& factors) {
	if (m_background) m_background->setScale(factors);
}