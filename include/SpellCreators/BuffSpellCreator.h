#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/BuffSpell.h"

// a class that creates buff spells. Can only be used by enemies.
class BuffSpellCreator final : virtual public SpellCreator {
public:
	BuffSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void execExecuteSpell(const sf::Vector2f& target) override;
};