#pragma once

#include "global.h"
#include "World/GameObject.h"

#include "GUI/BitmapText.h"
#include "Enums/SpellID.h"
#include "Structs/AttributeData.h"
#include "Structs/DamageOverTimeData.h"

// all spell ids are also a buff type,
// they are cast to int. That's why it starts at max.
enum class BuffType {
	Food = static_cast<int>(SpellID::MAX),
	PhysicalDamage,
	ShadowDamage,
	LightDamage,
	FireDamage,
	IceDamage,
	Stun,
	Fear
};

enum class BuffSlotType {
	Food,
	Spell,
	Dot,
	Debuff
};

class Spell;
class TooltipWindowComponent;

class BuffSlot final : public GameObject {
public:
	BuffSlot(BuffSlotType buffType, const sf::IntRect& textureLocation);

	// sets the spell attributes used by the tooltip.
	// can only be applied to BuffSlots of type "Spell"
	void setSpellAttributes(Spell* owner, const AttributeData& attributes, const sf::Time& duration);

	// sets the food attributes used by the tooltip.
	// can only be applied to BuffSlots of type "Food"
	void setFoodAttributes(const std::string& item_id, const AttributeData& attributes, const sf::Time& duration);

	// sets the DOT attributes used by the tooltip.
	// can only be applied to BuffSlots of type "Dot"
	void setDotAttributes(const DamageOverTimeData& data, const sf::Time& duration);

	// sets the debuff attributes used by the tooltip (stun and fear).
	// can only be applied to BuffSlots of type "Debuff"
	void setDebuffAttributes(const DamageOverTimeData& data, const sf::Time& duration);

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
	BuffSlotType m_buffType;

	sf::RectangleShape m_outside;
	sf::RectangleShape m_back;
	sf::RectangleShape m_inside;

	BitmapText m_durationText;

	std::vector<std::pair<sf::Time, int>> m_damageBeans;
	void updateDamageBeans(const sf::Time& frameTime);
	int m_currentDamage = 0;
	DamageType m_damageType = DamageType::VOID;

	// used for flashing
	static const sf::Time FLASHING_TIME;
	static const sf::Time FLASHING_INTERVAL;
	bool m_isVisible;
	sf::Time m_timeUntilFlash;
};