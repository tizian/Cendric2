#pragma once

#include "global.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Level/LevelInterface.h"

#include "GUI/ProgressLog.h"

// ancestor for level or map screen
class WorldScreen : public Screen {
public:
	WorldScreen(CharacterCore* core);
	virtual ~WorldScreen();

	void execUpdate(const sf::Time& frameTime) override;
	void execOnExit(const Screen* nextScreen) override;
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
	// notify description added -> add to core, display text
	void notifyQuestDescriptionAdded(const std::string& questID, int descriptionID);
	// notify condition added -> add to core, reload everything that needs a reload when conditions are added
	virtual void notifyConditionAdded(const std::string& conditionType, const std::string& condition);
	// getter for the inventory of the interface
	Inventory* getInventory();

protected:
	WorldInterface* m_interface = nullptr;
	ProgressLog* m_progressLog = nullptr;

	// For lighting
	sf::RenderTexture m_renderTexture;
    sf::RenderTexture m_renderTexture2;
	sf::Sprite m_sprite;
	sf::Shader m_lightLayerShader;
	sf::Shader m_foregroundLayerShader;

private:
	void updateOverlayQueue();
};