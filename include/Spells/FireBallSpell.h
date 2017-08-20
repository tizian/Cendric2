#pragma once

#include "global.h"
#include "Spells/Spell.h"

/*
the following skins are available:
0: fiery fireball (default)
1: icy fireball
2: shadow fireball
3: light fireball
4: energy fireball (from cairns)
5: janus' fireball
*/
class FireBallSpell final : public Spell {
public:
	FireBallSpell() {};
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) override;

private:
	void init(const SpellData& data);
};