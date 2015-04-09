#pragma once

#include "global.h"
#include "Spell.h"

class IceSpell : public Spell
{
public:
	void load() override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

private:
};