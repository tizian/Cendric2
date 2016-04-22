#pragma once

#include "global.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Level/LevelInterface.h"

#include "GUI/ProgressLog.h"

class Item;
class Trigger;

// ancestor for level or map screen
class WorldScreen : public Screen {
public:
	WorldScreen(CharacterCore* core);
	virtual ~WorldScreen();

	void execUpdate(const sf::Time& frameTime) override;
	void execOnExit(const Screen* nextScreen) override;
	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget);
	void updateProgressLog(const sf::Time& frameTime);

	// notify permanent item consumed -> change in core, display overlay and progresslog and reload inventory.
	void notifyPermanentItemConsumed(const Item& item);
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
	// notify spell learned -> add overlay and add to core.
	void notifySpellLearned(SpellID id);
	// notify modifier learned -> add overlay and add to core.
	void notifyModifierLearned(const SpellModifier& modifier);
	// notify reputation added
	void notifyReputationAdded(FractionID fraction, int amount);
	// reload all triggers, based on their conditions
	void reloadTriggers();
	// reloads a certain trigger
	void reloadTrigger(Trigger* trigger) const;
	// getter for the inventory of the interface
	Inventory* getInventory();
	// and for the main character
	virtual GameObject* getMainCharacter() const = 0;
	// and for the world
	virtual const World* getWorld() const = 0;
	// do everything thats needed before leaving the world screen and entering a new level / world. 
	// Return whether this is possible (it's not possible on a game over, for example)
	virtual bool exitWorld() = 0;
	// a return from the menu. reload a part of the configuration, restart music, etc.
	virtual void notifyBackFromMenu() = 0;

protected:
	// handle quicksave
	virtual void quicksave();
	// handle quickload 
	virtual void quickload();

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