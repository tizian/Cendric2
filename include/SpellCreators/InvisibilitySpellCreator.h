#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/InvisibilitySpell.h"

// a class that creates invisibility spells
class InvisibilitySpellCreator : public SpellCreator {
public:
	InvisibilitySpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f& target) override;

protected:
	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;

	int m_invisibilityLevel;
};