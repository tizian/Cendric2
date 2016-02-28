#pragma once

#include <map>

#include "global.h"
#include "GUI/BitmapText.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Enums/QuestState.h"

/* A screen overlay that supports arbitrary text, split in title and subtitle and/or a texture */
class ScreenOverlay : public virtual GameObject {
public:
	ScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
	virtual ~ScreenOverlay();

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;

	void setTexture(ResourceID texture);
	void setTitle(const std::string& textKey, const std::string& textType = "core");
	void setTitleRaw(const std::string& text);
	void setTitleCharacterSize(int characterSize);
	void setTitleColor(const sf::Color& color);
	void setSubtitle(const std::string& textKey, const std::string& textType = "core");
	void setSubtitleRaw(const std::string& text);
	void setSubtitleCharacterSize(int characterSize);
	void setSubtitleColor(const sf::Color& color);

	GameObjectType getConfiguredType() const override;

private:
	void load();
	void repositionText();

private:
	sf::Time m_activeTime;
	sf::Time m_fadeTime;
	sf::Time m_fadeInTimer;
	sf::Time m_fadeOutTimer;

	BitmapText m_title;
	BitmapText m_subtitle;
	sf::Sprite m_sprite;
};

class QuestScreenOverlay : public virtual ScreenOverlay {
public:
	QuestScreenOverlay(const std::string& questID, QuestState state);
};

class LocationScreenOverlay : public virtual ScreenOverlay {
public:
	LocationScreenOverlay(const std::string& locationKey);
};

class HintScreenOverlay : public virtual ScreenOverlay {
public:
	HintScreenOverlay(const std::string& hintKey);
};
