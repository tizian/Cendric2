#pragma once

#include "global.h"
#include "Spell.h"
#include "LightObject.h"

/*
projectiles act like chop spells, but they are ranged. Cendric will never have projectile spells.
*/
class ProjectileSpell : public Spell {
public:
	ProjectileSpell();
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

private:
};