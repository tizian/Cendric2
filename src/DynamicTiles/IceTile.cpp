#include "DynamicTiles/IceTile.h"
#include "Spell.h"

void IceTile::load(int skinNr)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
	m_isCollidable = true;

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_ice));
	idleAnimation.addFrame(sf::IntRect(BORDER, BORDER, m_tileSize.x, m_tileSize.y));
	idleAnimation.addFrame(sf::IntRect(BORDER + 1 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));
	
	addAnimation(GameObjectState::Idle, idleAnimation);

	setFrameTime(sf::seconds(0.5f));

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void IceTile::onHit(Spell* spell)
{
	switch (spell->getConfiguredSpellID())
	{
	case SpellID::Fire:
		spell->setDisposed();
		setDisposed();
		break;
	default:
		break;
	}
}