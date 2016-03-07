#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/InventorySlot.h"
#include "GUI/ItemDescriptionWindow.h"
#include "GUI/DocumentDescriptionWindow.h"
#include "GUI/TexturedTabBar.h"
#include "GUI/InventoryEquipment.h"

class LevelInterface;
class MapInterface;
class MerchantInterface;

// the inventory, as displayed in a level or a map
// it takes its information directly from the character core
// the inventory also shows item description and equipped items.
class Inventory {
public:
	Inventory(LevelInterface* _interface);
	Inventory(MapInterface* _interface);
	~Inventory();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void notifyChange(const std::string& itemID);
	void startTrading(MerchantInterface* _interface);
	void stopTrading();

	// fully reloads the inventory text & items, depending on the core. Heavy operation! Better use notifyChange if only one item changed.
	void reload();
	// reloads the inventory gold text
	void reloadGold();

private:
	CharacterCore* m_core;
	LevelInterface* m_levelInterface = nullptr;
	MapInterface* m_mapInterface = nullptr;
	MerchantInterface* m_merchantInterface = nullptr;
	bool m_isVisible = false;
	void init();

	void clearAllSlots();
	// reorganizes the positions of the 'slots' vector
	void calculateSlotPositions(std::map<std::string, InventorySlot>& slots);

	Window* m_window;

	BitmapText m_goldText;
	BitmapText m_selectedTabText;

	TexturedTabBar* m_tabBar;

	std::map<std::string, InventorySlot> m_consumableItems;
	std::map<std::string, InventorySlot> m_equipmentItems;
	std::map<std::string, InventorySlot> m_miscItems;
	std::map<std::string, InventorySlot> m_questItems;
	std::map<std::string, InventorySlot> m_documentItems;

	ItemType m_currentTab;
	// first is the id, the second is VOID when it is no equiment slot and an Item Type when it is an equipment slot
	std::pair<std::string, ItemType> m_selectedSlotId;
	void selectTab(ItemType type);
	// item type shall be VOID for not-equipment-slots
	void selectSlot(const std::string& selectedSlotId, ItemType type);
	void deselectCurrentSlot();
	InventorySlot* getSelectedSlot() const;

	void handleMapRightClick(InventorySlot* clicked);
	void handleLevelRightClick(InventorySlot* clicked);

	// used for drag & drop handling
	SlotClone* m_currentClone = nullptr;
	bool m_hasDraggingStarted = false;
	bool m_isEquipmentSlotDragged = false;
	bool m_isDragging = false;
	// the mouse has to move this distance while pressed to spawn a clone.
	const float DRAG_DISTANCE = 10.f;
	sf::Vector2f m_startMousePosition;
	void handleDragAndDrop();
	void removeEquipmentItem();
	void handleMapDrag();
	void handleLevelDrag();
	void handleMapDrop();
	void handleLevelDrop();

	ItemDescriptionWindow* m_descriptionWindow = nullptr;
	DocumentDescriptionWindow* m_documentWindow = nullptr;
	void showDescription(const Item& item);
	void hideDescription();
	void showDocument(const Item& item);
	void hideDocument();

	InventoryEquipment* m_equipment = nullptr;

	const int SLOT_COUNT_X = 5;
	const int SLOT_COUNT_Y = 5;
	const sf::Vector2f BUTTON_SIZE = sf::Vector2f(InventorySlot::SIZE + 10.f, 45.f);

	const float MARGIN = 7.f;
	const float INVENTORY_LEFT = GUIConstants::LEFT + MARGIN + InventoryEquipment::WIDTH;
	const float INVENTORY_HEIGHT =
		3 * GUIConstants::TEXT_OFFSET +
		2 * GUIConstants::CHARACTER_SIZE_M +
		BUTTON_SIZE.y +
		(SLOT_COUNT_Y + 1) * MARGIN +
		SLOT_COUNT_Y * (MARGIN + InventorySlot::SIZE);
	const float INVENTORY_WIDTH =
		2 * GUIConstants::TEXT_OFFSET +
		(SLOT_COUNT_X - 1) * MARGIN +
		SLOT_COUNT_X * InventorySlot::SIZE;

	std::map<ItemType, std::map<std::string, InventorySlot>*> m_typeMap;
};