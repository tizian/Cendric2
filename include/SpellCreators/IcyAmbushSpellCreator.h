#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/IcyAmbushSpell.h"

// a class that creates icy ambush spells
class IcyAmbushSpellCreator : public SpellCreator {
public:
	IcyAmbushSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;
	void executeSpell(const sf::Vector2f& target) override;

private:
	int m_strength = 1;

	void addStrengthModifier(int level) override;
	void addRangeModifier(int level) override;
	void addDurationModifier(int level) override;
};