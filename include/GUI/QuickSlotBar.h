#pragma once

#include "global.h"
#include "GUI/QuickSlot.h"

class WorldInterface;
class SlotClone;

// the quick slot bar in a level. There are two slots in total
class QuickSlotBar final {
public:
	QuickSlotBar(WorldInterface* _interface);
	~QuickSlotBar();

	void show();
	void hide();

	void highlightSlots(bool highlight) const;

	void render(sf::RenderTarget& target) const;
	void renderAfterForeground(sf::RenderTarget& target) const;
	void update(const sf::Time& frameTime) const;

	void notifyConsumableDrop(const SlotClone* item) const;
	void equipConsumable(const std::string& itemID) const;
	void reload() const;

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