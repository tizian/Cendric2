#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

// a spell creator that summons a mighty gargoyle
class SummonGargoyleSpellCreator : public SpellCreator {
public:
	SummonGargoyleSpellCreator(const SpellData &spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;

	void execExecuteSpell(const sf::Vector2f& target) override;

private:
	void addDurationModifier(int level) override;
};