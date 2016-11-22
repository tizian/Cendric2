#pragma once

#include "global.h"
#include "Spells/Spell.h"

// skins:
// 0: default, holy skin
// 1: shadow wisp skin 
// 2: fiery wisp skin
// 3: toxic wisp skin
class AureolaSpell : public Spell {
public:
	AureolaSpell() {}
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	void onOwnerDisposed() override;

private:
	// the range left the spell has to fly until it returns to its mob
	float m_rangeLeft;
	// is set to true when the spell is returning to its mob. It will follow the mob if this is true.
	bool m_isReturning = false;
	// this float is stored so it only needs to be calculated once.
	float m_absVel = 0.0f;
};