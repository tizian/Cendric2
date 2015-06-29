#include "Spells/ForcefieldSpell.h"

ForcefieldSpell::ForcefieldSpell() : Spell()
{
}

void ForcefieldSpell::load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target, int divergence)
{
	setSpriteOffset(sf::Vector2f(-12.f, -12.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_forcefield));
	spellAnimation.addFrame(sf::IntRect(0, 0, 120, 120));
	spellAnimation.addFrame(sf::IntRect(120, 0, 120, 120));
	spellAnimation.addFrame(sf::IntRect(240, 0, 120, 120));
	spellAnimation.addFrame(sf::IntRect(360, 0, 120, 120));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(level, mob, target, 0);
}

const sf::Vector2f ForcefieldSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(-49.f, 0.f);
}

float ForcefieldSpell::getConfiguredMaxVelocityY() const
{
	return 0.f;
}

float ForcefieldSpell::getConfiguredMaxVelocityX() const
{
	return 0.f;
}

bool ForcefieldSpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool ForcefieldSpell::getConfiguredRotateSprite() const
{
	return false;
}

SpellID ForcefieldSpell::getConfiguredSpellID() const
{
	return SpellID::Forcefield;
}