#include "Level/DynamicTiles/SpikesBottomTile.h"
#include "Spell.h"

void SpikesBottomTile::init() {
	setPositionOffset(sf::Vector2f(0.f, -25.f));
	setSpriteOffset(sf::Vector2f(0.f, -25.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 25.f));
}

void SpikesBottomTile::load(int skinNr) {

	Animation idleAnimation(sf::seconds(10.0f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_spikesbottom));
	idleAnimation.addFrame(sf::IntRect(0, (skinNr - 1) * m_tileSize.y, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void SpikesBottomTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_damageCooldown > sf::Time::Zero) {
		m_damageCooldown = m_damageCooldown - frameTime;
		if (m_damageCooldown < sf::Time::Zero) {
			m_damageCooldown = sf::Time::Zero;
		}
	}
}

void SpikesBottomTile::onHit(LevelMovableGameObject* mob) {
	if (m_damageCooldown == sf::Time::Zero) {
		mob->addDamage(DMG_PER_S);
		m_damageCooldown = sf::seconds(1);
	}
}