#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/InventorySlot.h"
#include "GUI/InventoryDescriptionWindow.h"
#include "GUI/TexturedButton.h"

class LevelMainCharacter;
class LevelInterface;

// the inventory, as displayed in a level or a map
// it takes its information directly from the character core
// the inventory also shows item description and equipped items.
class Inventory
{
public:
	Inventory(CharacterCore* core, LevelMainCharacter* character, LevelInterface* interface);
	~Inventory();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// reloads the inventory text & items, depending on the core
	void reload();

private:
	CharacterCore* m_core;
	LevelMainCharacter* m_character;
	LevelInterface* m_interface;
	bool m_isVisible = false;

	
	void clearAllSlots();
	// reorganizes the positions of the 'slots' vector
	void calculateSlotPositions(std::vector<InventorySlot>& slots);

	Window* m_window;

	BitmapText m_goldText;
	BitmapText m_selectedTabText;
	std::vector<std::pair<TexturedButton, ItemType>> m_tabs;

	std::vector<InventorySlot> m_consumableItems;
	std::vector<InventorySlot> m_equipmentItems;
	std::vector<InventorySlot> m_miscItems;
	std::vector<InventorySlot> m_questItems;
	std::vector<InventorySlot> m_documentItems;

	ItemType m_currentTab;
	InventorySlot* m_selectedSlot = nullptr;
	void selectTab(ItemType type);

	InventoryDescriptionWindow* m_descriptionWindow = nullptr;
	void showDescription(const InventorySlot& slot);
	void hideDescription();

	const int SLOT_COUNT_X = 5;
	const int SLOT_COUNT_Y = 5;
	const int CHARACTER_SIZE = 12;
	const sf::Vector2f BUTTON_SIZE = sf::Vector2f(InventorySlot::SIDE_LENGTH + 10.f, 35.f);

	const float TEXT_OFFSET = 20.f;
	const float MARGIN = 7.f;
	const float INVENTORY_TOP = 100.f;
	const float INVENTORY_LEFT = 50.f;
	const float INVENTORY_HEIGHT = 
		3 * TEXT_OFFSET + 
		2 * CHARACTER_SIZE + 
		BUTTON_SIZE.y +
		(SLOT_COUNT_Y + 1) * MARGIN +
		SLOT_COUNT_Y * (MARGIN + InventorySlot::SIDE_LENGTH + 2 * InventorySlot::MARGIN);
	const float INVENTORY_WIDTH = 
		2 * TEXT_OFFSET +
		(SLOT_COUNT_X - 1) * MARGIN +
		SLOT_COUNT_X * (InventorySlot::SIDE_LENGTH + 2 * InventorySlot::MARGIN);
	
	std::map<ItemType, std::vector<InventorySlot>*> m_typeMap;
};