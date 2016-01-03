#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/LeapOfFaithSpell.h"

// a class that creates leap of faith spells
class LeapOfFaithSpellCreator : public SpellCreator {
public:
	LeapOfFaithSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	void executeSpell(const sf::Vector2f& target) override;
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

private:
	int m_strength = 1;

	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;

	void removeOldSpell();
};