#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/FireBallSpell.h"

// a class that creates fireball spells
class FireBallSpellCreator : public SpellCreator {
public:
	FireBallSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;
};