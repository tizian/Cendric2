#include "Spells/IceSpell.h"

IceSpell::IceSpell()
{
}

void IceSpell::load(Level* level, LevelMovableGameObject* mob, sf::Vector2f target)
{
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));
	setBoundingBox(sf::FloatRect(0, 0, 10, 10));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_ice));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(level, mob, target);
}

float IceSpell::getConfiguredMaxVelocityY() const
{
	return 1000.0f;
}

float IceSpell::getConfiguredMaxVelocityX() const
{
	return 1000.0f;
}

bool IceSpell::getConfiguredTriggerFightAnimation() const
{
	return true;
}

SpellID IceSpell::getConfiguredSpellID() const
{
	return SpellID::Ice;
}

