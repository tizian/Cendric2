#pragma once

#include <map>

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/IceBallSpell.h"

// a class that creates iceball spells
class IceBallSpellCreator : public SpellCreator {
public:
	IceBallSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner);

	void executeSpell(const sf::Vector2f &target) override;
};