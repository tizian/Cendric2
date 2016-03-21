#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/TelekinesisSpell.h"

// a class that creates telekinesis spells
class TelekinesisSpellCreator : public SpellCreator {
public:
	TelekinesisSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;

private:
	void addRangeModifier(int level) override;
};