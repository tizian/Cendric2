#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/LeechSpell.h"

// a class that creates leech spells
class LeechSpellCreator : public SpellCreator
{
public:
	LeechSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;
};