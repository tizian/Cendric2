#pragma once

#include "global.h"
#include "Spells/Spell.h"

class LeechSpell final : public Spell {
public:
	LeechSpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;
	void onOwnerDisposed() override;

private:
	// is set to true when the spell is returning to its mob. It will follow the mob if this is true.
	bool m_isReturning = false;
	// this float is stored so it only needs to be calculated once.
	float m_absVel = 0.f;
};