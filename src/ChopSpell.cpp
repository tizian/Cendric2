#include "ChopSpell.h"

ChopSpell::ChopSpell()
{
	load();
}

void ChopSpell::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox (sf::FloatRect(0, 0, 40, 60));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_chop));
	spellAnimation.addFrame(sf::IntRect(0, 0, 40, 60));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

sf::Vector2f ChopSpell::getConfiguredPositionOffset()
{
	return sf::Vector2f(10.f, 0.f);
}

const float ChopSpell::getConfiguredMaxVelocityY()
{
	return 0.f;
}

const float ChopSpell::getConfiguredMaxVelocityX()
{
	return 0.f;
}

bool ChopSpell::getConfiguredIsAttachedToMainChar()
{
	return true;
}

bool ChopSpell::getConfiguredTriggerFightAnimation()
{
	return true;
}

SpellID ChopSpell::getConfiguredType()
{
	return SpellID::Chop;
}