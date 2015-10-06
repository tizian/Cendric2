#pragma once

#include "global.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "LevelInterface.h"

#include "GUI/ProgressLog.h"

// ancestor for level or map screen
class GameScreen : public Screen {
public:
	GameScreen(CharacterCore* core);
	virtual ~GameScreen();

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void updateProgressLog(const sf::Time& frameTime);

	// notify item change -> change in core, display text and reload inventory. Also reload quest log.
	void notifyItemChange(const std::string& itemID, int amount);
	// notify quest condition progress -> change in core, display text and reload quest log
	void notifyQuestConditionFulfilled(const std::string& questID, const std::string& condition);
	// notify quest target killed -> change in core, display text and reload quest log
	void notifyQuestTargetKilled(const std::string& questID, const std::string& name);
	// notify quest state changed -> change in core, display text and reload quest log
	void notifyQuestStateChanged(const std::string& questID, QuestState state);
	// getter for the inventory of the interface
	Inventory* getInventory();

protected:
	GameInterface* m_interface = nullptr;
	ProgressLog* m_progressLog = nullptr;

	// For lighting
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_sprite;
	sf::Shader m_lightLayerShader;
	sf::Shader m_foregroundLayerShader;
};