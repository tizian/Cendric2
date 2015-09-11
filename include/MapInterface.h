#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"

#include "GUI/Inventory.h"
#include "GUI/CharacterInfo.h"
#include "GUI/Spellbook.h"

// The interface overlay in a map
// if the keys for character screen, spellbook and inventory are pressed, these will display.
class MapInterface
{
public:
	MapInterface(CharacterCore* core);
	~MapInterface();

	// reloads the inventory if it is visible. 
	void reloadInventory();
	// an equipment item has been dropped. forward to equipment bar
	void notifyEquipmentDrop(const InventorySlotClone* item);
	// highlight equipment slots
	void highlightEquipmentSlots(bool highlight);

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	CharacterCore* getCore() const;

private:
	CharacterCore* m_core;

	// <<< INVENTORY >>>
	Inventory m_inventory;
	void updateInventory(const sf::Time& frameTime);

	// <<< SPELLBOOK >>>
	Spellbook m_spellbook;
	void updateSpellbook(const sf::Time& frameTime);

	// <<< CHARCTER INFO >>>
	CharacterInfo m_characterInfo;
	void updateCharacterInfo();
};