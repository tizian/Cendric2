#pragma once

#include "global.h"
#include "Spell.h"

class ChopSpell : public Spell
{
public:
	void load() override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

	sf::Vector2f getConfiguredPositionOffset() override;

private:
};