#include "Spells/ChopSpell.h"

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