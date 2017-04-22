#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/AureolaSpell.h"

// a class that creates aureola spells
class AureolaSpellCreator final : virtual public SpellCreator {
public:
	AureolaSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;

	void execExecuteSpell(const sf::Vector2f& target) override;

private:
	void addCountModifier(int level) override;
	void addDurationModifier(int level) override;
};