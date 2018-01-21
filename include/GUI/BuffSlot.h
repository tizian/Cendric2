#pragma once

#include "global.h"
#include "World/GameObject.h"

#include "GUI/BitmapText.h"
#include "ResourceManager.h"
#include "Enums/SpellID.h"
#include "Structs/AttributeData.h"
#include "GUI/Window.h"
#include "Structs/DamageOverTimeData.h"

enum class BuffType {
	Food,
	Spell,
	DamageOverTime
};

class Spell;
class TooltipWindowComponent;

class BuffSlot final : public GameObject {
public:
	BuffSlot(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration);

	// sets the spell attributes used by the tooltip.
	// can only be applied to BuffSlots of type "Spell"
	void setSpellAttributes(Spell* owner, const AttributeData& attributes);

	// sets the food attributes used by the tooltip.
	// can only be applied to BuffSlots of type "Food"
	void setFoodAttributes(const std::string& item_id, const AttributeData& attributes);

	// sets the DOT attributes used by the tooltip.
	// can only be applied to BuffSlots of type "Debuff"
	void setDotAttributes(const DamageOverTimeData& data);

	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;

	GameObjectType getConfiguredType() const override;
	SpellID getSpellID() const;
	void onRightClick() override;

	static const float MARGIN;
	static const float SIZE;

private:
	TooltipWindowComponent* m_tooltipComponent;
	sf::Time m_duration;
	Spell* m_ownerSpell = nullptr;
	BuffType m_buffType;

	sf::RectangleShape m_outside;
	sf::RectangleShape m_back;
	sf::RectangleShape m_inside;

	BitmapText m_durationText;

	// used for flashing
	static const sf::Time FLASHING_TIME;
	static const sf::Time FLASHING_INTERVAL;
	bool m_isVisible;
	sf::Time m_timeUntilFlash;
};