#include "ChopSpell.h"

void ChopSpell::load()
{
	m_activeCoolDown = TIME_ACTIVE;
	setSpriteOffset(sf::Vector2f(0.f, 0.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_chop));
	spellAnimation.addFrame(sf::IntRect(0, 0, 40, 60));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void ChopSpell::update(sf::Time& frameTime)
{
	Spell::update(frameTime);
	m_activeCoolDown = m_activeCoolDown - frameTime;
	if (m_activeCoolDown.asMilliseconds() <= 0)
	{
		setDisposed();
	}
}

sf::Vector2f ChopSpell::getConfiguredPositionOffset()
{
	return sf::Vector2f(10.f, 0.f);
}

const float ChopSpell::getConfiguredMaxVelocityY()
{
	return 100.0f;
}

const float ChopSpell::getConfiguredMaxVelocityX()
{
	return 100.0f;
}