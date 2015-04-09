#include "FireSpell.h"

void FireSpell::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));

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

const float FireSpell::getConfiguredMaxVelocityY()
{
	return 1000.0f;
}

const float FireSpell::getConfiguredMaxVelocityX()
{
	return 1000.0f;
}