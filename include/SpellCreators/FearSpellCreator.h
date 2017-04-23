#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/FearSpell.h"

// a class that creates fear spells
class FearSpellCreator final : public SpellCreator {
public:
	FearSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;

	void execExecuteSpell(const sf::Vector2f& target) override;

private:
	void addDurationModifier(int level) override;
};