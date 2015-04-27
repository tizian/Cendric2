#include "Spells\IceSpell.h"

IceSpell::IceSpell()
{
	load();
}

void IceSpell::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0, 0, 30, 30));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_ice));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

const float IceSpell::getConfiguredMaxVelocityY()
{
	return 1000.0f;
}

const float IceSpell::getConfiguredMaxVelocityX()
{
	return 1000.0f;
}

bool IceSpell::getConfiguredTriggerFightAnimation()
{
	return true;
}

SpellID IceSpell::getConfiguredType()
{
	return SpellID::Ice;
}

