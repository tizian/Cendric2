#pragma once

#include "global.h"
#include "Spells/Spell.h"

class FearSpell final : public Spell {
public:
	FearSpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
};