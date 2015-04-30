#pragma once

#include "global.h"
#include "Spell.h"

class ForcefieldSpell : public Spell
{
public:
	ForcefieldSpell();
	void load() override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

	sf::Vector2f getConfiguredPositionOffset() override;
	bool getConfiguredIsAttachedToMainChar() override;
	SpellID getConfiguredSpellID() override;

private:
};