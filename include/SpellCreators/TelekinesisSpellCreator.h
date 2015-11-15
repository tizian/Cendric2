#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/TelekinesisSpell.h"

// a class that creates telekinesis spells
class TelekinesisSpellCreator : public SpellCreator {
public:
	TelekinesisSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;

private:
	void addRangeModifier(int level) override;
};