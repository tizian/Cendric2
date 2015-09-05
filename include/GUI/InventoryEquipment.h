#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/InventorySlot.h"

class InventorySlotClone;

// the equipment part of the inventory. it shows all equipped items
class InventoryEquipment
{
public:
	InventoryEquipment(CharacterCore* core);
	~InventoryEquipment();

	void show();
	void hide();

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// reloads the equipment items, depending on the core
	void reload();
	// returns an inventory slot* if one was selected, else nullptr
	InventorySlot* getSelectedSlot();

	void highlightEquipmentSlot(ItemType type, bool highlight);
	// returns wheter an equipment change has happened.
	bool notifyEquipmentDrop(const InventorySlotClone* item);

	static float WIDTH;
	static float TOP;
	static float LEFT;
	static float HEIGHT;

private:
	CharacterCore* m_core;
	
	bool m_isVisible = false;
	
	std::map<ItemType, InventorySlot> m_slots;

	Window* m_window;
};