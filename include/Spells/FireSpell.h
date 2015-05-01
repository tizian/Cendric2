#pragma once

#include "global.h"
#include "Spell.h"

class FireSpell : public Spell
{
public:
	FireSpell();
	void load() override;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	bool getConfiguredTriggerFightAnimation() const override;
	SpellID getConfiguredSpellID() const override;

private:
};