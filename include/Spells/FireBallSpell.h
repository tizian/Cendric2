#pragma once

#include "global.h"
#include "Spells/Spell.h"
#include "LightObject.h"

/*
the following skins are available:
0: fiery fireball (default)
1: icy fireball
2: shadow fireball
3: light fireball
*/
class FireBallSpell : public Spell {
public:
	FireBallSpell() {};
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) override;

private:
	void init(const SpellData& data);
};