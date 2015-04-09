#pragma once

#include "global.h"
#include "Spell.h"

class ChopSpell : public Spell
{
public:
	void load() override;
	void update(sf::Time& frameTime) override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;

	sf::Vector2f getConfiguredPositionOffset() override;

private:
	const sf::Time TIME_ACTIVE = sf::milliseconds(200);
	sf::Time m_activeCoolDown;
};