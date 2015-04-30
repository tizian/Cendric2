#pragma once

#include "global.h"
#include "Spell.h"

class FireSpell : public Spell
{
public:
	FireSpell();
	void load() override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

	bool getConfiguredTriggerFightAnimation() override;
	SpellID getConfiguredSpellID() override;

private:
};