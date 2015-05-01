#include "Spells/FireSpell.h"

FireSpell::FireSpell()
{
	load();
}

void FireSpell::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0, 0, 30, 30));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fire));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
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