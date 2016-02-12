#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/ProjectileSpell.h"

// a class that creates projectile spells
class ProjectileSpellCreator : public SpellCreator {
public:
	ProjectileSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;
};