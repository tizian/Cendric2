#include "Level/DynamicTiles/ShootingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Shooting, ShootingTile)

ShootingTile::ShootingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen) {
}

void ShootingTile::init() {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));
	setPositionOffset(sf::Vector2f(10.f, 10.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F - 20.f, TILE_SIZE_F - 20.f));
	m_damage.damageType = DamageType::Physical;
	m_damage.duration = sf::seconds(4.f);
	m_damage.damage = 40;
	m_isAlwaysUpdate = true;
}

void ShootingTile::loadAnimation(int skinNr) {
	m_isCollidable = false;

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_jumping));
	for (int i = 0; i < 3; ++i) {
		idleAnimation->addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	if (skinNr == 2) { // fireball
		m_damage.damageType = DamageType::Fire;
		m_isMelting = true;
	}
}

void ShootingTile::onHit(LevelMovableGameObject* mob) {
	if (m_damageCooldown > sf::Time::Zero) return;
	if (mob->getConfiguredType() == GameObjectType::_LevelMainCharacter) {
		mob->addDamageOverTime(m_damage);
		m_damageCooldown = sf::seconds(3.f);
	}
}

void ShootingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	updateTime(m_damageCooldown, frameTime);

	if (m_isWaiting) {
		if (m_isAggro && dist(m_mainChar->getCenter(), getCenter()) > AGGRO_DISTANCE) {
			return;
		}
		updateTime(m_waitingTime, frameTime);
		if (m_waitingTime == sf::Time::Zero) {
			m_isWaiting = false;
		}
		return;
	}
}

void ShootingTile::onHit(Spell* spell) {
	// nop, yet
}


