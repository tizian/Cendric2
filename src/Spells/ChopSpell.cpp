#include "Spells/ChopSpell.h"

ChopSpell::ChopSpell()
{
	load();
}

void ChopSpell::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	// chop spell does not set bounding box here as it varies per weapon
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

bool ChopSpell::getConfiguredIsAttachedToMainChar() const
{
	return true;
}

bool ChopSpell::getConfiguredTriggerFightAnimation() const
{
	return true;
}

SpellID ChopSpell::getConfiguredSpellID() const
{
	return SpellID::Chop;
}