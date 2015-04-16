#pragma once

#include "global.h"
#include "Spell.h"

class ForcefieldSpell : public Spell
{
public:
	void load() override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

	sf::Vector2f getConfiguredPositionOffset() override;
	bool getConfiguredIsAttachedToMainChar() override;

private:
};