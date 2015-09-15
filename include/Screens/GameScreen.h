#pragma once

#include "global.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "LevelInterface.h"

#include "GUI/ProgressLog.h"

// ancestor for level or map screen
class GameScreen : public Screen
{
public:
	GameScreen(CharacterCore* core);
	virtual ~GameScreen();
	
	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	
	// notify item change -> change in core, display text and reload inventory
	void notifyItemChange(const std::string& itemID, int amount);
	// notify quest condition progress -> change in core, display text and reload quest log
	void notifyQuestConditionFulfilled(const std::string& questID, const std::string& condition);
	// notify quest target killed -> change in core, display text and reload quest log
	void notifyQuestTargetKilled(const std::string& questID, const std::string& name);

protected:
	GameInterface* m_interface = nullptr;

private:
	ProgressLog m_progressLog;
};