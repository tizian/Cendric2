#pragma once

#include <map>

#include "global.h"
#include "GUI/BitmapText.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Enums/QuestState.h"
#include "Enums/SpellID.h"
#include "Structs/SpellModifier.h"

class Item;

/* A screen overlay that supports arbitrary text, split in title and subtitle */
class ScreenOverlay : public virtual GameObject {
public:
	ScreenOverlay(const sf::Time& activeTime, const sf::Time& fadeTime = sf::Time::Zero);
	virtual ~ScreenOverlay() {};

	virtual void update(const sf::Time& frameTime) override;
	virtual void render(sf::RenderTarget& renderTarget) override;

	void setTitle(const std::string& textKey, const std::string& textType = "core");
	void setTitleRaw(const std::string& text);
	void setTitleCharacterSize(int characterSize);
	void setTitleColor(const sf::Color& color);
	void setSubtitle(const std::string& textKey, const std::string& textType = "core");
	void setSubtitleRaw(const std::string& text);
	void setSubtitleCharacterSize(int characterSize);
	void setSubtitleColor(const sf::Color& color);

	GameObjectType getConfiguredType() const override;

	static ScreenOverlay* createQuestScreenOverlay(const std::string& questID, QuestState state);
	static ScreenOverlay* createLocationScreenOverlay(const std::string& locationKey);
	// returns nullptr if the configuration for display hints is false.
	static ScreenOverlay* createHintScreenOverlay(const std::string& hintKey);
	static ScreenOverlay* createPermanentItemScreenOverlay(const Item& item);
	static ScreenOverlay* createSpellLearnedScreenOverlay(SpellID id);
	static ScreenOverlay* createModifierLearnedLearnedScreenOverlay(const SpellModifier& modifier);

protected: 
	float m_scale;

	BitmapText m_title;
	BitmapText m_subtitle;

	virtual void repositionText();

private:
	sf::Time m_activeTime;
	sf::Time m_fadeTime;
	sf::Time m_fadeInTimer;
	sf::Time m_fadeOutTimer;

	void load();
};
