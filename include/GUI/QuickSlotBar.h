#pragma once

#include "global.h"
#include "GUI/QuickSlot.h"
#include "GUI/InventorySlotClone.h"

class LevelInterface;

// the quick slot bar in a level. There are two slots in total
class QuickSlotBar
{
public:
	QuickSlotBar(LevelInterface* _interface);
	~QuickSlotBar();

	void show();
	void hide();

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void notifyConsumableDrop(const InventorySlotClone* item);
	void reload();

private:
	bool m_isVisible = true;

	QuickSlot* m_quickSlot1 = nullptr;
	QuickSlot* m_quickSlot2 = nullptr;

	// x space between two slots
	const float QUICKSLOT_SPACING = 20.f;
	// as seen from the top left corner
	const sf::Vector2f QUICKSLOT_OFFSET = sf::Vector2f(
		WINDOW_WIDTH - 2 * (QuickSlot::SIDE_LENGTH + QUICKSLOT_SPACING), 
		WINDOW_HEIGHT - (QuickSlot::SIDE_LENGTH + QUICKSLOT_SPACING));
	
};