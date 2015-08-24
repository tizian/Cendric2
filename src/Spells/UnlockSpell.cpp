#include "Spells/UnlockSpell.h"

UnlockSpell::UnlockSpell() : Spell()
{
}

void UnlockSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	Spell::load(bean, mob, target);
}

const sf::Vector2f UnlockSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(10.f, 0.f);
}

bool UnlockSpell::getConfiguredRotateSprite() const
{
	return false;
}