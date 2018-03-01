#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/IcyAmbushSpell.h"

// a class that creates icy ambush spells
class IcyAmbushSpellCreator final : public SpellCreator {
public:
	IcyAmbushSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	void execExecuteSpell(const sf::Vector2f& target) override;

private:
	void addRangeModifier(int level) override;
	void addDurationModifier(int level) override;
	void addSpeedModifier(int level) override;
};