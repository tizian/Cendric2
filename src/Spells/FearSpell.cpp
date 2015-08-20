#include "Spells/FearSpell.h"

FearSpell::FearSpell(const sf::Time& fearedDuration, int strength) : Spell()
{
	m_fearedDuration = fearedDuration;
	m_strength = strength;
}

void FearSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target)
{
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fear));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);
}

void FearSpell::execOnHit(LevelMovableGameObject *target)
{
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) 
	{
		if (enemy->getMentalStrength() <= m_strength)
		{
			enemy->setFeared(m_fearedDuration);
			setDisposed();
		}
	}
	// main character can't be feared yet.
}