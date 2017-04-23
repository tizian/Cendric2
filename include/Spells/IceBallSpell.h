#pragma once

#include "global.h"
#include "Spells/Spell.h"

// skins:
// 0: default iceball
// 1: janus' iceball
class IceBallSpell final : public Spell {
public:
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
};