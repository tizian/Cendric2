#pragma once

#include "global.h"
#include "Spell.h"

class ForcefieldSpell : public Spell
{
public:
	ForcefieldSpell();
	void load() override;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredIsAttachedToMainChar() const override;
	SpellID getConfiguredSpellID() const override;

private:
};