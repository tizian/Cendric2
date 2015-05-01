#pragma once

#include "global.h"
#include "Spell.h"

class IceSpell : public Spell
{
public:
	IceSpell();
	void load() override;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	bool getConfiguredTriggerFightAnimation() const override;
	SpellID getConfiguredSpellID() const override;

private:
};