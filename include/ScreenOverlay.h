#pragma once

#include <map>

#include "global.h"
#include "GUI/BitmapText.h"
#include "GameObject.h"
#include "ResourceManager.h"

/* A screen overlay that supports arbitrary text and optionally a texture */
class ScreenOverlay : public virtual GameObject {
public:
	ScreenOverlay(const std::string& text, const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
	ScreenOverlay(ResourceID texture, const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
	ScreenOverlay(const std::string& text, ResourceID texture, const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
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
	void centerText();

private:
	sf::Time m_activeTime;
	sf::Time m_fadeTime;
	sf::Time m_fadeInTimer;
	sf::Time m_fadeOutTimer;

	BitmapText m_bitmapText;
	sf::Sprite m_sprite;
};

