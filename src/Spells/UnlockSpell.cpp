#include "Spells/UnlockSpell.h"

UnlockSpell::UnlockSpell() : Spell()
{
}

void UnlockSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target, float divergenceAngle)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	Spell::load(bean, mob, target, 0);
}

const sf::Vector2f UnlockSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(10.f, 0.f);
}

bool UnlockSpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool UnlockSpell::getConfiguredRotateSprite() const
{
	return false;
}