#include "Level/DynamicTiles/JumpingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Jumping, JumpingTile)

JumpingTile::JumpingTile(LevelScreen* levelScreen) :
	MovableGameObject(),
	LevelDynamicTile(levelScreen) {
}

void JumpingTile::init() {
	setSpriteOffset(sf::Vector2f(-3.f, -3.f));
	setPositionOffset(sf::Vector2f(-3.f, -3.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F - 6.f, TILE_SIZE_F - 6.f));
	m_damage.damageType = DamageType::Physical;
	m_damage.duration = sf::seconds(4.f);
	m_damage.damage = 40;
	m_isAlwaysUpdate = true;
}

void JumpingTile::loadAnimation(int skinNr) {
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

	if (skinNr == 2) // fireball
		m_damage.damageType = DamageType::Fire;
}

void JumpingTile::onHit(LevelMovableGameObject* mob) {
	if (m_damageCooldown > sf::Time::Zero) return;
	if (mob->getConfiguredType() == GameObjectType::_LevelMainCharacter) {
		mob->addDamageOverTime(m_damage);
		m_damageCooldown = sf::seconds(3.f);
	}
}

void JumpingTile::setJumpingTileData(const JumpingTileData& data) {
	m_isAggro = data.isAggro;
	m_isAlternating = data.isAlternating;

	float phi = degToRad(static_cast<float>(data.direction - 90));

	m_initialVelocity.x = std::round(data.velocity * std::cos(phi));
	m_initialVelocity.y = std::round(data.velocity * std::sin(phi));

	m_waitingSpan = sf::milliseconds(data.waitingTime);
	m_initialPosition = data.spawnPosition;
}

void JumpingTile::changeDirection() {
	if (m_isAlternating && m_isReturning) {
		setVelocity(sf::Vector2f(-m_initialVelocity.x, m_initialVelocity.y));
		m_animatedSprite.setFlippedY(false);
		m_isReturning = false;
	}
	else {
		setVelocity(m_initialVelocity);
		setPosition(m_initialPosition);
		m_animatedSprite.setFlippedY(true);
		m_isReturning = true;
	}
}

void JumpingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	updateTime(m_damageCooldown, frameTime);

	if (m_isWaiting) {
		if (m_isAggro && dist(m_mainChar->getCenter(), getCenter()) > AGGRO_DISTANCE) {
			return;
		}
		updateTime(m_waitingTime, frameTime);
		if (m_waitingTime == sf::Time::Zero) {
			m_isWaiting = false;
			changeDirection();
		}
		return;
	}

	if (m_velocity.y > 0.f) {
		// falling down
		if (getPosition().y >= m_initialPosition.y) {
			if (m_waitingSpan > sf::Time::Zero) {
				m_isWaiting = true;
				m_waitingTime = m_waitingSpan;
				setVelocity(sf::Vector2f(0.f, 0.f));
				return;
			}
			changeDirection();
		}
	}
	
	setAcceleration(sf::Vector2f(0.f, GRAVITY_ACCELERATION));
	
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);

	// rotate sprite
	setSpriteRotation(atan2(m_velocity.y, m_velocity.x));
}

void JumpingTile::onHit(Spell* spell) {
	// nop, yet
}

void JumpingTile::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::Vector2f oldPosition = getPosition();
	const sf::FloatRect& bb = *getBoundingBox();
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	WorldCollisionQueryRecord rec;

	rec.excludedGameObject = this;

	bool isMovingDown = nextPosition.y > bb.top;

	// check for collision on y axis
	rec.boundingBox = nextBoundingBoxY;
	rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
	rec.movingParent = nullptr;
	bool collidesY = m_level->collides(rec);
	setMovingParent(rec.movingParent);
	if (collidesY) {
		setAccelerationY(0.f);
		setVelocityY(0.f);
		setPositionY(rec.safeTop);
	}
}


