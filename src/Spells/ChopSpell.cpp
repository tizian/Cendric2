#include "Spells/ChopSpell.h"

ChopSpell::ChopSpell()
{
}

void ChopSpell::load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	// chop spell does not set bounding box here as it varies per weapon
	Spell::load(level, mob, target);
}

const sf::Vector2f ChopSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(10.f, 0.f);
}

float ChopSpell::getConfiguredMaxVelocityY() const
{
	return 0.f;
}

float ChopSpell::getConfiguredMaxVelocityX() const
{
	return 0.f;
}

bool ChopSpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool ChopSpell::getConfiguredTriggerFightAnimation() const
{
	return true;
}

bool ChopSpell::getConfiguredRotateSprite() const
{
	return false;
}

SpellID ChopSpell::getConfiguredSpellID() const
{
	return SpellID::Chop;
}