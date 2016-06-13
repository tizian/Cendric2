#pragma once

#include "global.h"
#include "BitmapText.h"
#include "TextProvider.h"
#include "GUIConstants.h"
#include "Enums/EnumNames.h"

class CharacterCore;
class Item;

class ProgressLogEntry {
public:
	ProgressLogEntry();
	~ProgressLogEntry();

	void updateBottom(const sf::Time& frameTime);
	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& renderTarget);

	void setPosition(const sf::Vector2f& position);
	void setAlpha(float alpha);

	void forceRemove();

	float getHeight() const;
	sf::Time getScrollTime() const;
	inline bool isOver() const { return m_scrollTimer <= sf::Time::Zero; }

	static ProgressLogEntry* createItemEntry(const std::string& str, const sf::Color& color, const std::string &itemID);
	static ProgressLogEntry* createQuestEntry(const std::string& str, const sf::Color& color);
	static ProgressLogEntry* createReputationEntry(const std::string& str, const sf::Color& color);

public:
	static const float ENTRY_SPACING;

	// how long can a single entry live?
	static const sf::Time TIME_TO_LIVE;
	static const sf::Time TIME_TO_FADE;
	static const sf::Time TIME_TO_SCROLL;

private:
	sf::Time m_time;
	sf::Time m_fadeInTimer;
	sf::Time m_fadeOutTimer;
	sf::Time m_scrollTimer;
	BitmapText* m_text = nullptr;
	sf::RectangleShape* m_icon = nullptr;
	sf::RectangleShape* m_background = nullptr;
	sf::RectangleShape* m_border = nullptr;

	static const float BORDER_SIZE;
	static const float ICON_SIZE;
	static const float ICON_OFFSET;
	static const float ICON_MARGIN;
};

// a small visual interface to show progress in a screen, such as 
// item, gold or quest changes
class ProgressLog {
public:
	ProgressLog(const CharacterCore* core);
	~ProgressLog();
	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& renderTarget);

	void addPermanentItemProgress(const Item& item);
	void addItemProgress(const std::string& itemID, int amount);
	void addQuestConditionFullfilled(const std::string& questID, const std::string& condition);
	void addQuestTargetKilled(const std::string& questID, const std::string& name);
	void addQuestStateChanged(const std::string& questID, QuestState state);
	void addQuestDescriptionAdded(const std::string& questID);
	void addReputationAdded(FractionID fraction, int amount);

	void setYOffset(float yOffset);
	void setVisible(bool visible);

private:
	bool m_isVisible = true;
	// a vector filled with texts (and their time to live) that log progress
	std::vector<ProgressLogEntry*> m_logTexts;
	// the core to calculate the correct number of killed targets
	const CharacterCore* m_core;

	// start text offset from the bottom of the screen
	float m_yOffset = 80.f;

	void calculatePositions();

	sf::Time m_scrollTime;

	// text offset from the left of the screen
	static const float XOFFSET;
	// soft cap on number of entries
	static const int MAX_ENTRIES;
};