#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/FlashSpell.h"

// a class that creates flash spells
class FlashSpellCreator : public SpellCreator {
public:
	FlashSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;
};