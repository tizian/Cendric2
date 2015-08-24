#pragma once

#include "global.h"
#include "Spell.h"

class DivineShieldSpell : public Spell
{
public:
	DivineShieldSpell(int additionalResistance);
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;

	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredRotateSprite() const override;

private:
	AttributeBean m_additionalResistance;
};