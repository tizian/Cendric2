#pragma once

#include "global.h"
#include "Spells/Spell.h"

/*
projectiles act like chop spells, but they are ranged. Cendric will never have projectile spells.
projectile spells are not only used by MOBS but also by LevelDynamicTiles, thats why they have a second load method.
*/
class ProjectileSpell final : public Spell {
public:
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) override;

private:
	void init(const SpellData& data);
};