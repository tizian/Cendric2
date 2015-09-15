#pragma once

#include "global.h"
#include "BitmapText.h"
#include "TextProvider.h"
#include "GUIConstants.h"

// a small visual interface to show progress in a screen, such as 
// item or gold changes, or quest changes
class ProgressLog 
{
public:
	ProgressLog();
	~ProgressLog();
	void update(const sf::Time& frameTime);
	void render(sf::RenderTarget& renderTarget);

	void addItemProgress(const std::string& itemID, int amount);
	void addQuestProgress(const std::string& questID);

private:
	// a vector filled with texts (and their time to live) that log progress
	std::vector<std::pair<BitmapText, sf::Time>> m_logTexts;

	void calculatePositions();

	// how long can a single entry live?
	const sf::Time TIME_TO_LIVE = sf::seconds(2);
	// start text offset from the top of the screen
	const float YOFFSET = 200.f;
	// text offset from the right of the screen
	const float XOFFSET = 20.f;
};