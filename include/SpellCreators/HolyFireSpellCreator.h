#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/HolyFireSpell.h"

// a class that creates holy fire spells
class HolyFireSpellCreator : public SpellCreator {
public:
	HolyFireSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	void executeSpell(const sf::Vector2f& target) override;

private:
	void addDurationModifier(int level) override;
	void addRangeModifier(int level) override;
};