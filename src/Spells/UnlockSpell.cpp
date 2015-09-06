#include "Spells/UnlockSpell.h"

UnlockSpell::UnlockSpell(int strength) : Spell()
{
	m_strength = strength;
}

void UnlockSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target)
{
	setSpriteOffset(sf::Vector2f(-10.f, 0.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_unlock));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 10));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(10.f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);

	Spell::load(bean, mob, target);
}