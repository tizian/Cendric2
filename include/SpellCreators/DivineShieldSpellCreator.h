#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/DivineShieldSpell.h"

// a class that creates divine shield spells
class DivineShieldSpellCreator : public SpellCreator {
public:
	DivineShieldSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f& target) override;

protected:
	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;
	int m_additionalResistance;
};