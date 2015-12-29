#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/ShackleSpell.h"

// a class that creates shackle spells
class ShackleSpellCreator : public SpellCreator {
public:
	ShackleSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f &target) override;

private:
	int m_strength = 0;

	void addDamageModifier(int level) override;
	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;
};