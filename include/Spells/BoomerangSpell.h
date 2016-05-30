#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "LightObject.h"

/*
this projectile will fly in a circle and return to its owner.
*/
class BoomerangSpell : public Spell {
public:
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

private:
	void init(const SpellData& data);
};