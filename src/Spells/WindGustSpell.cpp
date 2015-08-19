#include "Spells/WindGustSpell.h"

WindGustSpell::WindGustSpell() : Spell()
{
}

void WindGustSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target, float divergenceAngle)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	// TODO: this spell needs a particle system!
	Spell::load(bean, mob, target, 0);
}

const sf::Vector2f WindGustSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(10.f, 20.f);
}

bool WindGustSpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool WindGustSpell::getConfiguredRotateSprite() const
{
	return false;
}