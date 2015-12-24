#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/LightSpell.h"

// a class that creates light spells
class LightSpellCreator : public SpellCreator {
public:
	LightSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	void executeSpell(const sf::Vector2f& target) override;

private:
	void addDurationModifier(int level) override;
};