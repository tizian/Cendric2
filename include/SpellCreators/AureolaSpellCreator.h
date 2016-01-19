#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/AureolaSpell.h"

// a class that creates aureola spells
class AureolaSpellCreator : public SpellCreator {
public:
	AureolaSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f &target) override;

private:
	int m_strength = 1;

	void addStrengthModifier(int level) override;
	void addCountModifier(int level) override;
	void addDamageModifier(int level) override;
	void addDurationModifier(int level) override;
};