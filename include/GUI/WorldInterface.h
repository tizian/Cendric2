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
#include "GUI/MapOverlay.h"

class WorldScreen;

enum class GUIElement {
	VOID = -1,
	Character = 0,
	Inventory,
	Spellbook,
	Journal,
	Map
};

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
	void loadGuiSidebar();
	void loadMapSidebar();

protected:
	WorldScreen* m_screen;
	CharacterCore* m_core;

	GUITabBar* m_guiSidebar = nullptr;
	GUITabBar* m_mapSidebar = nullptr;
	Inventory* m_inventory = nullptr;
	CharacterInfo* m_characterInfo = nullptr;
	Spellbook* m_spellbook = nullptr;
	QuestLog* m_questLog = nullptr;
	MapOverlay* m_mapOverlay = nullptr;

	template<typename G>
	void updateGuiElement(const sf::Time& frameTime, G* guiElement, GUIElement type);

	static Key getKeyFromGuiElement(GUIElement e);
};