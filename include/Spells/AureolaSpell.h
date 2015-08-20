#pragma once

#include "global.h"
#include "Spell.h"

class AureolaSpell : public Spell
{
public:
	AureolaSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	const sf::Vector2f getConfiguredPositionOffset() const override;

private:
	// the range left the spell has to fly until it returns to its mob
	float m_rangeLeft;
	// is set to true when the spell is returning to its mob. It will follow the mob if this is true.
	bool m_isReturning = false;
	// this float is stored so it only needs to be calculated once.
	float m_absVel = 0.0f;
};