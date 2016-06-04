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
	ProgressLogEntry(const std::string& str, const sf::Color& color);
	ProgressLogEntry(const std::string& str, const sf::Color& color, const std::string &itemID);
	~ProgressLogEntry();

	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& renderTarget);
	void setPosition(const sf::Vector2f& position);

	float getHeight() const;
	inline sf::Time getTime() const { return m_time; }

public:
	static const float ENTRY_SPACING;

private:
	sf::Time m_time;
	BitmapText* m_text = nullptr;
	sf::RectangleShape* m_icon = nullptr;
	sf::RectangleShape* m_background = nullptr;
	sf::RectangleShape* m_border = nullptr;

	// how long can a single entry live?
	static const float TIME_TO_LIVE;
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

private:
	// a vector filled with texts (and their time to live) that log progress
	std::vector<ProgressLogEntry*> m_logTexts;
	// the core to calculate the correct number of killed targets
	const CharacterCore* m_core;

	// start text offset from the bottom of the screen
	float m_yOffset = 80.f;

	void calculatePositions();

	// text offset from the left of the screen
	const float XOFFSET = 20.f;
};