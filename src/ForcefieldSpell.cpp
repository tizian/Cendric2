#include "ForcefieldSpell.h"

void ForcefieldSpell::load()
{
	setSpriteOffset(sf::Vector2f(-12.f, -12.f));
	setBoundingBox(sf::FloatRect(0, 0, 98, 98));

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
}

sf::Vector2f ForcefieldSpell::getConfiguredPositionOffset()
{
	return sf::Vector2f(-49.f, 0.f);
}

const float ForcefieldSpell::getConfiguredMaxVelocityY()
{
	return 0.f;
}

const float ForcefieldSpell::getConfiguredMaxVelocityX()
{
	return 0.f;
}

bool ForcefieldSpell::getConfiguredIsAttachedToMainChar()
{
	return true;
}