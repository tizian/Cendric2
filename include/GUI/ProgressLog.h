#pragma once

#include "global.h"
#include "BitmapText.h"
#include "TextProvider.h"
#include "GUIConstants.h"
#include "Enums/EnumNames.h"

class CharacterCore;

// a small visual interface to show progress in a screen, such as 
// item, gold or quest changes
class ProgressLog {
public:
	ProgressLog(const CharacterCore* core);
	~ProgressLog();
	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& renderTarget);

	void addItemProgress(const std::string& itemID, int amount);
	void addQuestConditionFullfilled(const std::string& questID, const std::string& condition);
	void addQuestTargetKilled(const std::string& questID, const std::string& name);
	void addQuestStateChanged(const std::string& questID, QuestState state);

private:
	// a vector filled with texts (and their time to live) that log progress
	std::vector<std::pair<BitmapText, sf::Time>> m_logTexts;
	// the core to calculate the correct number of killed targets
	const CharacterCore* m_core;

	void calculatePositions();

	// how long can a single entry live?
	const sf::Time TIME_TO_LIVE = sf::seconds(2);
	// start text offset from the top of the screen
	const float YOFFSET = 20.f;
	// text offset from the right of the screen
	const float XOFFSET = 20.f;
};