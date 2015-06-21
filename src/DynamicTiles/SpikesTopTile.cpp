#include "DynamicTiles/SpikesTopTile.h"
#include "Spell.h"

void SpikesTopTile::load(int skinNr)
{
	setSpriteOffset(sf::Vector2f(-5.f, -20.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 65.f));
	m_isCollidable = false;

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_spikestop));
	idleAnimation.addFrame(sf::IntRect(0, 0, m_tileSize.x, m_tileSize.y * 2));

	addAnimation(GameObjectState::Idle, idleAnimation);

	setFrameTime(sf::seconds(10.0f));

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

void SpikesTopTile::update(const sf::Time& frameTime)
{
	DynamicTile::update(frameTime);
	if (m_damageCooldown > sf::Time::Zero)
	{
		m_damageCooldown = m_damageCooldown - frameTime;
		if (m_damageCooldown < sf::Time::Zero)
		{
			m_damageCooldown = sf::Time::Zero;
		}
	}
}

void SpikesTopTile::onHit(LevelMovableGameObject* mob)
{
	if (m_damageCooldown == sf::Time::Zero)
	{
		mob->addDamage(DMG_PER_S);
		m_damageCooldown = sf::seconds(1);
	}
}

void SpikesTopTile::onHit(Spell* spell)
{
	// TODO, maybe there is some spell that alters spikes?
}