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
	int textureHeight = 2 * m_tileSize.y;

	Animation burningAnimation;
	burningAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_torch));
	burningAnimation.addFrame(sf::IntRect(0, (skinNr - 1) * textureHeight, m_tileSize.x, textureHeight));
	burningAnimation.addFrame(sf::IntRect(m_tileSize.x, (skinNr - 1) * textureHeight, m_tileSize.x, textureHeight));
	burningAnimation.addFrame(sf::IntRect(m_tileSize.x * 2, (skinNr - 1) * textureHeight, m_tileSize.x, textureHeight));
	burningAnimation.addFrame(sf::IntRect(m_tileSize.x * 3, (skinNr - 1) * textureHeight, m_tileSize.x, textureHeight));

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
	switch (spell->getSpellID())
	{
	case SpellID::Chop:
		spell->setDisposed();
		setDisposed();
		break;
	case SpellID::IceBall:
		if (m_state == GameObjectState::Burning)
		{
			m_state = GameObjectState::Idle;
			setCurrentAnimation(getAnimation(m_state), false);
			spell->setDisposed();
		}
		break;
	case SpellID::FireBall:
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