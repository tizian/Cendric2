#pragma once

#include "global.h"
#include "Spell.h"

class ChopSpell : public Spell
{
public:
	ChopSpell();
	void load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target, int divergence) override;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredIsAttachedToMob() const override;
	bool getConfiguredTriggerFightAnimation() const override;
	bool getConfiguredRotateSprite() const override;
	SpellID getConfiguredSpellID() const override;
	DamageType getConfiguredDamageType() const override;

private:
};