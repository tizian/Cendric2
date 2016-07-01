#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "Window.h"
#include "GUI/InventorySlot.h"
#include "GUI/MerchantItemDescriptionWindow.h"
#include "GUI/Button.h"
#include "GUI/SlicedSprite.h"

class MerchantInterface;
class ScrollBar;
class ScrollHelper;

// the merchant window, operating on a merchant interface
class MerchantWindow {
public:
	MerchantWindow(MerchantInterface* _interface);
	~MerchantWindow();

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void notifyChange(const std::string& itemID);

private:
	MerchantInterface* m_interface;
	std::string m_id;

	void init();

	// reloads the merchant items, depending on the core
	void reload();
	void clearAllSlots();
	void completeTrade();
	// reorganizes the positions of the 'm_items' map
	void calculateSlotPositions();

	Window* m_window;
	BitmapText m_title;

	SlicedSprite m_scrollWindow;
	ScrollBar* m_scrollBar = nullptr;
	ScrollHelper* m_scrollHelper = nullptr;

	std::string m_selectedSlotId = "";
	void selectSlot(const std::string& selectedSlotId);
	void deselectCurrentSlot();
	InventorySlot* getSelectedSlot();

	std::map<std::string, InventorySlot> m_items;
	MerchantItemDescriptionWindow* m_descriptionWindow = nullptr;

	void showDescription(const Item* item);
	void hideDescription();

	static const int SLOT_COUNT_X;
	static const int SLOT_COUNT_Y;

	static const float ICON_MARGIN;
	static const float WINDOW_MARGIN;

	static const float SCROLL_WINDOW_LEFT;
	static const float SCROLL_WINDOW_TOP;
	static const float SCROLL_WINDOW_WIDTH;
	static const float SCROLL_WINDOW_HEIGHT;

	static const float WIDTH;
	static const float TOP;
	static const float LEFT;
};