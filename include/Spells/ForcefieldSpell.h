#pragma once

#include "global.h"
#include "Spell.h"

class ForcefieldSpell : public Spell
{
public:
	ForcefieldSpell();
	void load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target);

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	const sf::Vector2f getConfiguredPositionOffset() const override;
	bool getConfiguredIsAttachedToMob() const override;
	bool getConfiguredRotateSprite() const override;
	SpellID getConfiguredSpellID() const override;

private:
};