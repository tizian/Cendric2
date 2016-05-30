#pragma once

#include "ScreenOverlays/ScreenOverlay.h"

/* A screen overlay that additionally supports up to two textures */
class TextureScreenOverlay : public virtual ScreenOverlay {
public:
	TextureScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void setSpriteTexture(const sf::Texture* texture);
	void setSpriteTextureRect(const sf::IntRect& rect);
	void setSpriteTextureColor(const sf::Color& color);
	void setSpritePosition(const sf::Vector2f& position);
	void setSpriteScale(const sf::Vector2f& factors);

	void setBackgroundTexture(const sf::Texture* texture);
	void setBackgroundTextureRect(const sf::IntRect& rect);
	void setBackgroundColor(const sf::Color& color);
	void setBackgroundPosition(const sf::Vector2f& position);
	void setBackgroundScale(const sf::Vector2f& factors);

private:	
	sf::Sprite m_sprite;
	sf::Sprite m_background;
};
