#include "Spells/AureolaSpell.h"

AureolaSpell::AureolaSpell() : Spell()
{
}

void AureolaSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, sf::Vector2f target, float divergenceAngle)
{
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_aureola));
	spellAnimation.addFrame(sf::IntRect(0, 0, 40, 40));
	spellAnimation.addFrame(sf::IntRect(40, 0, 40, 40));

	addAnimation(GameObjectState::Idle, spellAnimation);

	setFrameTime(sf::seconds(0.3f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	m_rangeLeft = bean.range;

	Spell::load(bean, mob, target, divergenceAngle);
}

void AureolaSpell::update(const sf::Time& frameTime)
{
	calculateNextPosition(frameTime, m_nextPosition);

	if (m_rangeLeft > 0.f)
	{
		sf::Vector2f div = m_nextPosition - getPosition();
		m_rangeLeft -= std::sqrt(div.x * div.x + div.y * div.y);
		if (m_rangeLeft <= 0.f)
		{
			m_rangeLeft = -1.f;
			setVelocityX(-getVelocity().x);
			setVelocityY(-getVelocity().y);
			setRotation(atan2(getVelocity().y, getVelocity().x));
		}
	}

	// check collisions with main char
	if (m_ownerType != GameObjectType::_MainCharacter)
	{
		checkCollisionsWithMainChar(getBoundingBox());
	}
	// check collisions with enemies
	checkCollisionsWithEnemies(getBoundingBox());
	MovableGameObject::update(frameTime);

	m_activeCoolDown = m_activeCoolDown - frameTime;
	
	if (m_activeCoolDown.asMilliseconds() <= 0)
	{
		setDisposed();
	}
}

const sf::Vector2f AureolaSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(5.f, 5.f);
}
