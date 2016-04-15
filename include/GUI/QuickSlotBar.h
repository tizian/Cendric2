#pragma once

#include "global.h"
#include "GUI/QuickSlot.h"

class LevelInterface;
class SlotClone;

// the quick slot bar in a level. There are two slots in total
class QuickSlotBar {
public:
	QuickSlotBar(LevelInterface* _interface);
	~QuickSlotBar();

	void show();
	void hide();

	void highlightSlots(bool highlight);

	void render(sf::RenderTarget& target);
	void renderAfterForeground(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	void notifyConsumableDrop(const SlotClone* item);
	void reload();

private:
	bool m_isVisible = true;

	QuickSlot* m_quickSlot1 = nullptr;
	QuickSlot* m_quickSlot2 = nullptr;

	// x space between two slots
	const float QUICKSLOT_SPACING = 20.f;
	// as seen from the top left corner
	const sf::Vector2f QUICKSLOT_OFFSET = sf::Vector2f(
		WINDOW_WIDTH - 2 * (QuickSlot::SIZE + QUICKSLOT_SPACING),
		WINDOW_HEIGHT - (QuickSlot::SIZE + QUICKSLOT_SPACING));

};