#pragma once

#include "global.h"
#include "Spell.h"

class AureolaSpell : public Spell
{
public:
	AureolaSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, sf::Vector2f target, float divergenceAngle) override;
	void update(const sf::Time& frameTime) override;

	const sf::Vector2f getConfiguredPositionOffset() const override;

private:
	// the range left the spell has to fly until it returns to our main char
	float m_rangeLeft;
};