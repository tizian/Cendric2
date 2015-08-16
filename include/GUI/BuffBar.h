#pragma once

#include "global.h"
#include "GUI/BuffSlot.h"

// the buff bar in a level
class BuffBar
{
public:
	BuffBar();
	~BuffBar();

	void show();
	void hide();
	// called by the spell. the buff type determines the texture & how the slot looks
	void addSlot(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration);

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// the maximal buffs this interface shows. if there are more, it doesn't display them all.
	static const int MAX_SHOWABLE_BUFFSLOTS;

private:
	bool m_isVisible = true;

	// the buff slots for spells are seperated from the one for food.
	// there can only be one food buff at a time.
	std::vector<BuffSlot*> m_buffSlots;
	BuffSlot* m_foodBuffSlot = nullptr;

	void calculateSlotPositions();

	// as seen from the top left corner
	const sf::Vector2f BUFFBAR_OFFSET = sf::Vector2f(WINDOW_WIDTH - 10.f, 10.f);
	// x space between two slots
	const float BUFFSLOT_SPACING = 20.f;
};