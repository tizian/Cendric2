#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/ShadowTrapSpell.h"

// a class that creates shadow trap spells
class ShadowTrapSpellCreator : public SpellCreator {
public:
	ShadowTrapSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f &target) override;

private:
	int m_strength = 1;

	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;
	void addDamageModifier(int level) override;
};