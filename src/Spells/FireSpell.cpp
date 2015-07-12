#include "Spells/FireSpell.h"

FireSpell::FireSpell() : Spell()
{
}

void FireSpell::load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target, int divergence)
{
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fire));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(level, mob, target, divergence);
}

float FireSpell::getConfiguredMaxVelocityY() const
{
	return 1000.0f;
}

float FireSpell::getConfiguredMaxVelocityX() const
{
	return 1000.0f;
}

bool FireSpell::getConfiguredTriggerFightAnimation() const
{
	return true;
}

SpellID FireSpell::getConfiguredSpellID() const
{
	return SpellID::Fire;
}

DamageType FireSpell::getConfiguredDamageType() const
{
	return DamageType::Fire;
}