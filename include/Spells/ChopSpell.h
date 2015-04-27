#pragma once

#include "global.h"
#include "Spell.h"

class ChopSpell : public Spell
{
public:
	ChopSpell();
	void load() override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

	sf::Vector2f getConfiguredPositionOffset() override;
	bool getConfiguredIsAttachedToMainChar() override;
	bool getConfiguredTriggerFightAnimation() override;
	SpellID getConfiguredType() override;

private:
};