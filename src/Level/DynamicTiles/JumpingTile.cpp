#include "Level/DynamicTiles/JumpingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"
#include "GameObjectComponents/LightComponent.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Jumping, JumpingTile)

JumpingTile::JumpingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen),
	MovableGameObject() {
	m_isAlwaysUpdate = true;
}

bool JumpingTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));
	setPositionOffset(sf::Vector2f(10.f, 10.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F - 20.f, TILE_SIZE_F - 20.f));
	m_damage.damageType = DamageType::Physical;
	m_damage.duration = sf::seconds(4.f);
	m_damage.damage = 10;

	int direction = 0;
	if (contains(properties, std::string("direction"))) {
		direction = std::stoi(properties.at(std::string("direction"))) % 360;
	}

	if (!contains(properties, std::string("velocity"))) return false;
	int velocity = std::stoi(properties.at(std::string("velocity")));
	if (velocity > 1000 || velocity < 100) return false;

	if (!contains(properties, std::string("waiting"))) return false;
	int waiting = std::stoi(properties.at(std::string("waiting")));
	if (waiting < 100 || waiting > 10000) return false;

	m_isAggro = contains(properties, std::string("aggro"));
	m_isAlternating = contains(properties, std::string("alternating"));

	float phi = degToRad(static_cast<float>(direction - 90));

	m_initialVelocity.x = std::round(velocity * std::cos(phi));
	m_initialVelocity.y = std::round(velocity * std::sin(phi));

	m_waitingSpan = sf::milliseconds(waiting);
	m_initialPosition = getPosition();

	return true;
}

void JumpingTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 3; ++i) {
		idleAnimation->addFrame(sf::IntRect(TILE_SIZE * i, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	if (skinNr == 1) { // fireball
		m_damage.damageType = DamageType::Fire;
		m_isMelting = true;
		addComponent(new LightComponent(LightData(
			sf::Vector2f(TILE_SIZE * 0.5f, TILE_SIZE * 0.5f),
			sf::Vector2f(100.f, 150.f)), this));
	}
	else if (skinNr == 2 || skinNr == 3) { // glowing magic-infused shadow piranha or toxic ball
		m_damage.damageType = DamageType::Shadow;
		addComponent(new LightComponent(LightData(
			sf::Vector2f(TILE_SIZE * 0.5f, TILE_SIZE * 0.5f), 
			sf::Vector2f(100.f, 100.f), 
			0.3f), this));
	}
}

void JumpingTile::onHit(LevelMovableGameObject* mob) {
	if (m_damageCooldown > sf::Time::Zero) return;
	if (mob->getConfiguredType() == GameObjectType::_LevelMainCharacter) {
		mob->addDamageOverTime(m_damage);
		m_damageCooldown = sf::seconds(3.f);
	}
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
	if (collidesY && getVelocity().y < 0.f) {
		setAccelerationY(0.f);
		setVelocityY(0.f);
		setPositionY(rec.safeTop);
	}

	if (collidesY && m_isMelting) {
		for (auto& it : *m_level->getDynamicTiles()) {
			LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
			if (tile == nullptr || tile->getDynamicTileID() != LevelDynamicTileID::Ice) continue;
			const sf::FloatRect& tileBB = *tile->getBoundingBox();
			if (nextBoundingBoxY.intersects(tileBB)) {
				tile->setDisposed();
			}
		}
	}
}

std::string JumpingTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_jumping.png";
}
