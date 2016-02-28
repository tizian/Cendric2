#pragma once

#include <map>

#include "global.h"
#include "GUI/BitmapText.h"
#include "GameObject.h"
#include "ResourceManager.h"

/* A screen overlay sprite that supports arbitrary text and optionally a texture */
class ScreenOverlay : public virtual GameObject {
public:
	// an active time of zero signalizes that this overlay shall be active until it's set disposed
	ScreenOverlay(const std::string& text, const sf::Time& activeTime, const sf::Time& fadeTime = sf::seconds(0.f));
	ScreenOverlay(ResourceID texture, const sf::Time& activeTime, const sf::Time& fadeTime = sf::seconds(0.f));
	ScreenOverlay(const std::string& text, ResourceID texture, const sf::Time& activeTime, const sf::Time& fadeTime = sf::seconds(0.f));
	~ScreenOverlay();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void setTexture(ResourceID texture);
	void setText(const std::string& text);
	void setTextSize(int characterSize);
	void setTextColor(const sf::Color& color);

	GameObjectType getConfiguredType() const override;

private:
	void load();

private:
	sf::Time m_activeTime;
	sf::Time m_fadeTime;
	sf::Time m_fadeInTimer;
	sf::Time m_fadeOutTimer;

	BitmapText m_bitmapText;
	sf::Sprite m_sprite;
	
};

