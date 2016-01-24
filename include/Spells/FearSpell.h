#pragma once

#include "global.h"
#include "Spell.h"

class FearSpell : public Spell {
public:
	FearSpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
};