#pragma once

#include "global.h"
#include "Spell.h"

class IceBallSpell : public Spell {
public:
	IceBallSpell();
	void load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
};