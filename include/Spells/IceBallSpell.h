#pragma once

#include "global.h"
#include "Spell.h"

class IceBallSpell : public Spell
{
public:
	IceBallSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, sf::Vector2f target, float divergenceAngle) override;
};