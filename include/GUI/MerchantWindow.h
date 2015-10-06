#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "Window.h"
#include "GUI/InventorySlot.h"
#include "GUI/MerchantItemDescriptionWindow.h"
#include "GUI/Button.h"

class MerchantInterface;

// the merchant window, operating on a merchant interface
class MerchantWindow {
public:
	MerchantWindow(MerchantInterface* _interface);
	~MerchantWindow();

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void notifyChange(const std::string& itemID);

private:
	MerchantInterface* m_interface;
	Button* m_cancelButton = nullptr;
	std::string m_id;

	void init();

	// reloads the merchant items, depending on the core
	void reload();
	void clearAllSlots();
	// reorganizes the positions of the 'slots' vector
	void calculateSlotPositions(std::vector<InventorySlot>& slots);

	Window* m_window;
	BitmapText m_title;

	std::vector<InventorySlot> m_items;
	InventorySlot* m_selectedSlot = nullptr;
	void selectSlot(InventorySlot* selectedSlot);
	MerchantItemDescriptionWindow* m_descriptionWindow = nullptr;

	void showDescription(const Item& item);
	void hideDescription();

	const int SLOT_COUNT_X = 5;
	const int SLOT_COUNT_Y = 5;

	const float MARGIN = 7.f;
	const float TOP = GUIConstants::TOP;
	const float WIDTH =
		2 * GUIConstants::TEXT_OFFSET +
		(SLOT_COUNT_X - 1) * MARGIN +
		SLOT_COUNT_X * (InventorySlot::SIDE_LENGTH + 2 * InventorySlot::MARGIN);
	const float LEFT = WINDOW_WIDTH - WIDTH - GUIConstants::LEFT;
	const float HEIGHT =
		5 * GUIConstants::TEXT_OFFSET +
		2 * GUIConstants::CHARACTER_SIZE_M +
		(SLOT_COUNT_Y + 1) * MARGIN +
		SLOT_COUNT_Y * (MARGIN + InventorySlot::SIDE_LENGTH + 2 * InventorySlot::MARGIN);
};