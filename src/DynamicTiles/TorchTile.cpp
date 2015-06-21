#include "DynamicTiles/TorchTile.h"
#include "Spell.h"

void TorchTile::init()
{
	setSpriteOffset(sf::Vector2f(-10.f, -static_cast<float>(m_tileSize.y) / 2));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x) / 2.f, static_cast<float>(m_tileSize.y)));
}

void TorchTile::load(int skinNr)
{
	m_isCollidable = false;

	Animation burningAnimation;
	burningAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_torch));
	burningAnimation.addFrame(sf::IntRect(0, 0, m_tileSize.x, 2 * m_tileSize.y));
	burningAnimation.addFrame(sf::IntRect(m_tileSize.x, 0, m_tileSize.x, 2 * m_tileSize.y));
	burningAnimation.addFrame(sf::IntRect(m_tileSize.x * 2, 0, m_tileSize.x, 2 * m_tileSize.y));
	burningAnimation.addFrame(sf::IntRect(m_tileSize.x * 3, 0, m_tileSize.x, 2 * m_tileSize.y));

	addAnimation(GameObjectState::Burning, burningAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_torch));
	idleAnimation.addFrame(sf::IntRect(m_tileSize.x * 4, 0, m_tileSize.x, 2 * m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	setFrameTime(sf::seconds(0.2f));

	// initial values
	m_state = GameObjectState::Burning;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void TorchTile::onHit(Spell* spell)
{
	switch (spell->getConfiguredSpellID())
	{
	case SpellID::Chop:
		spell->setDisposed();
		setDisposed();
		break;
	case SpellID::Ice:
		if (m_state == GameObjectState::Burning)
		{
			m_state = GameObjectState::Idle;
			setCurrentAnimation(getAnimation(m_state), false);
			spell->setDisposed();
		}
		break;
	case SpellID::Fire:
		if (m_state == GameObjectState::Idle)
		{
			m_state = GameObjectState::Burning;
			setCurrentAnimation(getAnimation(m_state), false);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}