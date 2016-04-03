#pragma once

#include "ScreenOverlays/ScreenOverlay.h"

/* A screen overlay that supports additionally a texture */
class TextureScreenOverlay : public virtual ScreenOverlay {
public:
	TextureScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void setTexture(ResourceID texture);
	void setTextureRect(const sf::IntRect& rect);
	void setTextureColor(const sf::Color& color);
	void setSpritePosition(const sf::Vector2f& position);
	void setSpriteScale(const sf::Vector2f& factors);

private:	
	sf::Color m_textureColor;
	sf::Sprite m_sprite;
};
