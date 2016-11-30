#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"

#include "GUI/Inventory.h"
#include "GUI/CharacterInfo.h"
#include "GUI/Spellbook.h"
#include "GUI/QuestLog.h"
#include "GUI/GUITabBar.h"

class WorldScreen;

// abstract class for an interface in level or map
class WorldInterface {
public:
	WorldInterface(WorldScreen* screen);
	virtual ~WorldInterface();

	// reloads the inventory for the items that have changed. if the string equals "gold", reloads gold
	// if the string is empty, it does a full reload (heavy operation, only us this when equipment or more than one item changed)
	virtual void reloadInventory(const std::string& changeditemID = "");
	// reloads the quest log
	virtual void reloadQuestLog();
	// reload the character info
	virtual void reloadCharacterInfo();
	// reload the spellbook
	virtual void reloadSpellBook();

	virtual void render(sf::RenderTarget& target);
	virtual void renderAfterForeground(sf::RenderTarget& target);
	virtual void update(const sf::Time& frameTime);
	virtual void hideAll();
	// returns whether any gui overlay is open (inventory, spellbook, etc.)
	virtual bool isGuiOverlayVisible() const;

	CharacterCore* getCore() const;
	virtual WorldScreen* getScreen() const;
	Inventory* getInventory() const;

protected:
	WorldScreen* m_screen;
	CharacterCore* m_core;

	// <<< GUI TAB SIDEBAR >>>
	GUITabBar* m_sidebar = nullptr;
	void updateSidebar(const sf::Time& frameTime);

	// <<< INVENTORY >>>
	Inventory* m_inventory = nullptr;
	void updateInventory(const sf::Time& frameTime);

	// <<< CHARCTER INFO >>>
	CharacterInfo* m_characterInfo = nullptr;
	void updateCharacterInfo(const sf::Time& frameTime);
	
	// <<< SPELLBOOK >>>
	Spellbook* m_spellbook = nullptr;
	void updateSpellbook(const sf::Time& frameTime);

	// <<< QUEST LOG >>>
	QuestLog* m_questLog = nullptr;
	void updateQuestLog(const sf::Time& frameTime);
};