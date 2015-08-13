#include "Spells/AntiGravitySpell.h"

AntiGravitySpell::AntiGravitySpell() : Spell()
{
}

void AntiGravitySpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target, float divergenceAngle)
{
	Spell::load(bean, mob, target, 0);
	m_mob->flipGravity();
}

void AntiGravitySpell::render(sf::RenderTarget& target)
{
	// nop ... yet.
}

void AntiGravitySpell::update(const sf::Time& frameTime)
{
	calculatePositionAccordingToMob(m_nextPosition);
	setPosition(m_nextPosition);

	m_duration = m_duration - frameTime;

	if (m_duration.asMilliseconds() <= 0)
	{
		m_mob->flipGravity();
		setDisposed();
	}
}

const sf::Vector2f AntiGravitySpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(-49.f, 0.f);
}

bool AntiGravitySpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool AntiGravitySpell::getConfiguredRotateSprite() const
{
	return false;
}