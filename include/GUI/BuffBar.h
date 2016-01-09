#pragma once

#include "global.h"
#include "GUI/BuffSlot.h"
#include "Enums/SpellID.h"
#include "Structs/DamageOverTimeData.h"

class LevelInterface;

// the buff bar in a level
class BuffBar {
public:
	BuffBar(LevelInterface* _interface);
	~BuffBar();

	void show();
	void hide();

	void addSpellBuff(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr);
	void addFoodBuff(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr);
	void addDotBuff(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data);
	// removes all buffs with the referenced spell id
	void removeTypedSpellBuffs(SpellID id);

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

	void calculateBuffPositions();

	// as seen from the top left corner
	const sf::Vector2f BUFFBAR_OFFSET = sf::Vector2f(220.f, 10.f);
	// x space between two slots
	const float BUFFSLOT_SPACING = 20.f;

	LevelInterface* m_interface = nullptr;
	bool m_notifyInterface = false;
};