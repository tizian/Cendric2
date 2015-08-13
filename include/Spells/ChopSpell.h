#pragma once

#include "global.h"
#include "Spell.h"

class ChopSpell : public Spell
{
public:
	ChopSpell();
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target, float divergenceAngle) override;

	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredIsAttachedToMob() const override;
	bool getConfiguredRotateSprite() const override;

private:
};