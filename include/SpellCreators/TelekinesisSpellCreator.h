#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/TelekinesisSpell.h"

// a class that creates telekinesis spells
class TelekinesisSpellCreator final : public SpellCreator {
public:
	TelekinesisSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void execExecuteSpell(const sf::Vector2f& target) override;

private:
	void addRangeModifier(int level) override;
	void addSpeedModifier(int level) override;
};