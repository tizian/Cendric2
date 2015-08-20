#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/FireBallSpell.h"

// a class that creates fireball spells
class FireBallSpellCreator : public SpellCreator 
{
public:
	FireBallSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner);
 
	void executeSpell(const sf::Vector2f& target) override;
};