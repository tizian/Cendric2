#pragma once

#include "global.h"
#include "Spells/Spell.h"

class UnlockSpell final : public Spell {
public:
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
};