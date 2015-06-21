#pragma once

#include "global.h"
#include "Spell.h"

class UnlockSpell : public Spell
{
public:
	UnlockSpell();
	void load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target) override;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredIsAttachedToMob() const override;
	bool getConfiguredTriggerFightAnimation() const override;
	bool getConfiguredRotateSprite() const override;
	SpellID getConfiguredSpellID() const override;

private:
};