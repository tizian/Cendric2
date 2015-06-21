#include "Spells/UnlockSpell.h"

UnlockSpell::UnlockSpell()
{
}

void UnlockSpell::load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	Spell::load(level, mob, target);
}

const sf::Vector2f UnlockSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(10.f, 0.f);
}

float UnlockSpell::getConfiguredMaxVelocityY() const
{
	return 0.f;
}

float UnlockSpell::getConfiguredMaxVelocityX() const
{
	return 0.f;
}

bool UnlockSpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool UnlockSpell::getConfiguredTriggerFightAnimation() const
{
	return false;
}

bool UnlockSpell::getConfiguredRotateSprite() const
{
	return false;
}

SpellID UnlockSpell::getConfiguredSpellID() const
{
	return SpellID::Unlock;
}