#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "SpellManager.h"
#include "InputController.h"
#include "ResourceManager.h"

#include "GUI/HealthBar.h"
#include "GUI/Inventory.h"
#include "GUI/CharacterInfo.h"
#include "GUI/SpellSelection.h"
#include "GUI/BuffBar.h"
#include "GUI/QuickSlotBar.h"
#include "GUI/Spellbook.h"
#include "GUI/QuestLog.h"

// The interface overlay in a level, displaying hp bar, spell cooldowns, buffs, quickslot bar
// if the keys for character screen and inventory are pressed, these will display as well.
class LevelInterface
{
public:
	LevelInterface(CharacterCore* core, LevelMainCharacter* character);
	~LevelInterface();

	// the spell manager of the level main character.
	void setSpellManager(SpellManager* spellManager);

	// used to add buffs to the buff bar
	void addBuff(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration);
	// reloads the inventory if it is visible. also reloads the quick slot bar
	void reloadInventory();
	// an consumable item has been dropped. forward to quick slot bar
	void notifyConsumableDrop(const InventorySlotClone* item);
	// consumes a consumable item
	void consumeItem(const Item& item);
	// highlight quickslots
	void highlightQuickslots(bool highlight);

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	CharacterCore* getCore() const;
	LevelMainCharacter* getMainCharacter() const;

private:
	CharacterCore* m_core;
	LevelMainCharacter* m_character;

	// <<< INVENTORY >>>
	Inventory m_inventory;
	void updateInventory(const sf::Time& frameTime);

	// <<< CHARCTER INFO >>>
	CharacterInfo m_characterInfo;
	void updateCharacterInfo();

	// <<< SPELLBOOK >>>
	Spellbook m_spellbook;
	void updateSpellbook(const sf::Time& frameTime);

	// <<< QUEST LOG >>>
	QuestLog m_questLog;
	void updateQuestLog(const sf::Time& frameTime);
	
	// <<< HEALTH BAR >>>
	HealthBar m_healthBar;

	// <<< SPELL SELECTION >>>
	SpellSelection* m_spellSelection = nullptr;

	// <<< QUICK SLOTS >>>
	QuickSlotBar m_quickSlotBar;

	// <<< BUFF BAR >>>
	BuffBar m_buffBar;
};