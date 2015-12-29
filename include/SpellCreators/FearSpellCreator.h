#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/FearSpell.h"

// a class that creates fear spells
class FearSpellCreator : public SpellCreator {
public:
	FearSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f &target) override;

private:
	int m_strength = 0;

	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;
};