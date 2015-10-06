#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/IceBallSpell.h"

// a class that creates iceball spells
class IceBallSpellCreator : public SpellCreator {
public:
	IceBallSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner);

	void executeSpell(const sf::Vector2f &target) override;
};