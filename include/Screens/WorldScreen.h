#pragma once

#include "global.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Level/LevelInterface.h"
#include "World/WeatherSystem.h"
#include "GUI/ProgressLog.h"
#include "Structs/Condition.h"

class Item;
class Trigger;
class BookWindow;

// ancestor for level or map screen
class WorldScreen : public virtual Screen {
public:
	WorldScreen(CharacterCore* core);
	virtual ~WorldScreen();

	void execUpdate(const sf::Time& frameTime) override;
	void execOnExit(const Screen* nextScreen) override;
	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget);
	void updateProgressLog(const sf::Time& frameTime);
	void setBook(const Item& document, bool isReopenInventory = false);

	// a quickslot has been assigned.
	void notifyQuickSlotAssignment(const std::string& itemId, int quickslotNr);
	// notify permanent item consumed -> change in core, display overlay and progresslog and reload inventory.
	void notifyPermanentItemConsumed(const Item* item);
	// notify item change -> change in core, display text and reload inventory. Also reload quest log and check for monitored quest items.
	void notifyItemChange(const std::string& itemID, int amount);
	// notify item conversion -> change in core, display text and reload inventory. Also reload quest log and check for monitored quest items.
	void notifyItemConversion(const std::string& oldItemID, const std::string& newItemID, int amount);
	// notify item equip -> change in core, reload inventory and character info.
	// if the second argument is not given (void, default), it gets determined in this method
	virtual void notifyItemEquip(const std::string& itemID, ItemType type = ItemType::VOID);
	// notify quest condition progress -> change in core, display text and reload quest log
	void notifyQuestConditionFulfilled(const std::string& questID, const std::string& condition);
	// notify quest target killed -> change in core, display text and reload quest log
	void notifyQuestTargetKilled(const std::string& questID, const std::string& name);
	// notify quest state changed -> change in core, display text and reload quest log. Also update quest items to monitor
	void notifyQuestStateChanged(const std::string& questID, QuestState state);
	// notify description added -> add to core, display text
	void notifyQuestDescriptionAdded(const std::string& questID, int descriptionID);
	// notify condition added -> add to core, reload everything that needs a reload when conditions are added
	virtual void notifyConditionAdded(const Condition& condition);
	// notify spell learned -> add overlay and add to core.
	void notifySpellLearned(SpellID id);
	// notify modifier learned -> add overlay and add to core.
	void notifyModifierLearned(SpellModifierType modifierType, int objectID);
	// notify reputation added
	void notifyReputationAdded(FractionID fraction, int amount);
	// notify guild joined
	void notifyGuildSet(FractionID fraction);
	// notify reputation added
	void notifyHintAdded(const std::string& hintKey);
	// notifies that the godmode property has changed
	virtual void toggleGodmode();
	// reload all triggers, based on their conditions
	void reloadTriggers();
	// reloads a certain trigger
	void reloadTrigger(Trigger* trigger) const;
	// getter for the inventory of the interface
	Inventory* getInventory();
	// getter for the progress log of the interface
	ProgressLog* getProgressLog();
	// and for the main character
	virtual MainCharacter* getMainCharacter() const = 0;
	// and for the world
	virtual const World* getWorld() const = 0;
	// and for the world data
	virtual const WorldData* getWorldData() const = 0;
	// do everything thats needed before leaving the world screen and entering a new level / world. 
	// Return whether this is possible (it's not possible on a game over, for example)
	virtual bool exitWorld() = 0;
	// a return from the menu. reload a part of the configuration, restart music, etc.
	virtual void notifyBackFromMenu() = 0;
	// add an overlay to the overlay queue they will be displayed within the next world screen
	void addScreenOverlay(ScreenOverlay* overlay, bool force = false);

protected:
	// handle quicksave
	virtual void quicksave();
	// handle quickload 
	virtual void quickload();

protected:
	WorldInterface* m_interface = nullptr;
	ProgressLog* m_progressLog = nullptr;
	std::vector<ScreenOverlay*> m_overlayQueue;

	// For lighting
	sf::RenderTexture m_renderTexture;
	sf::RenderTexture m_renderTexture2;
	sf::Sprite m_sprite;
	sf::Shader m_lightLayerShader;
	sf::Shader m_foregroundLayerShader;
	static const std::string VERTEX_SHADER;

	// weather
	WeatherSystem* m_weatherSystem = nullptr;
	void loadWeather();

	// book / document window
	void handleBookWindow(const sf::Time& frameTime);
	BookWindow* m_bookWindow = nullptr;
	bool m_bookWindowDisposed = false;
	bool m_isReopenInventory = false;

	// quest item monitoring
	std::map<std::string, std::set<std::string>> m_monitoredQuestItems;
	void updateMonitoredQuestItems();
	void checkMonitoredQuestItems(const std::string& itemID, int amount);

private:
	void updateOverlayQueue();
	void clearOverlayQueue();
};