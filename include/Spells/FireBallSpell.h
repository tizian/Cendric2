#pragma once

#include "global.h"
#include "Spell.h"

class FireBallSpell : public Spell
{
public:
	FireBallSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, sf::Vector2f target, float divergenceAngle) override;
};