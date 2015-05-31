#include "DynamicTiles/CrumblyBlockTile.h"
#include "Spell.h"

void CrumblyBlockTile::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
	m_isCollidable = true;

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_crumblyblock));
	idleAnimation.addFrame(sf::IntRect(BORDER, BORDER, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation crumblingAnimation;
	crumblingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_crumblyblock));
	crumblingAnimation.addFrame(sf::IntRect(BORDER + 1 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));
	crumblingAnimation.addFrame(sf::IntRect(BORDER + 2 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));
	crumblingAnimation.addFrame(sf::IntRect(BORDER + 3 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));
	crumblingAnimation.addFrame(sf::IntRect(BORDER + 4 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Crumbling, crumblingAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void CrumblyBlockTile::update(const sf::Time& frameTime)
{
	if (m_state == GameObjectState::Crumbling)
	{
		m_crumblingTime = m_crumblingTime - frameTime;
		if (m_crumblingTime < sf::Time::Zero)
		{
			setDisposed();
		}
	}
	DynamicTile::update(frameTime);
}

void CrumblyBlockTile::onHit(Spell* spell)
{
	switch (spell->getConfiguredSpellID())
	{
	case SpellID::Chop:
		if (m_state == GameObjectState::Idle)
		{
			m_state = GameObjectState::Crumbling;
			setCurrentAnimation(getAnimation(m_state), false);
			m_isCollidable = false;
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}