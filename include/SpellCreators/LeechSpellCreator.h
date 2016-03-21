#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/LeechSpell.h"

// a class that creates leech spells
class LeechSpellCreator : public SpellCreator {
public:
	LeechSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;
};