#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/InventorySlot.h"

class InventorySlot;
class SlotClone;
class WorldScreen;

// the equipment part of the inventory. it shows all equipped items
class InventoryEquipment {
public:
	InventoryEquipment(WorldScreen* screen);
	~InventoryEquipment();

	void show();
	void hide();

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// reloads the equipment items, depending on the core
	void reload();
	// checks if the equipment part of the inventory needs a reload and sets the bool to false again (!)
	bool requiresReload();
	// returns an inventory slot* if one was selected, else nullptr
	InventorySlot* getSelectedSlot();
	// returns an inventory slot* of the selected type, could be nullptr.
	InventorySlot* getSelectedSlot(ItemType type);

	void highlightEquipmentSlot(ItemType type, bool highlight);
	void notifyEquipmentDrop(const SlotClone* item);
	void equipItem(const InventorySlot* slot);

	static float WIDTH;

private:
	CharacterCore* m_core;
	WorldScreen* m_screen;

	bool m_isVisible = false;
	bool m_requiresReload = false;
	bool m_isInLevel = false;

	std::map<ItemType, InventorySlot> m_slots;

	Window* m_window;
};