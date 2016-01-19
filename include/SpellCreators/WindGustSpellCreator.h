#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/WindGustSpell.h"

// a class that creates windgust spells
class WindGustSpellCreator : public SpellCreator {
public:
	WindGustSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f &target) override;

private:
	void addRangeModifier(int level) override;
	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;
	void addDamageModifier(int level) override;

	int m_strength = 1;
};