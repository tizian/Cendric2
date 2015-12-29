#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/AntiGravitySpell.h"

// a class that creates anti gravity spells
class AntiGravitySpellCreator : public SpellCreator {
public:
	AntiGravitySpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;
private:
	void addDurationModifier(int level) override;
};