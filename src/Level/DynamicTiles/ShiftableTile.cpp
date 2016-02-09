#include "Level/DynamicTiles/ShiftableTile.h"
#include "Spell.h"
#include "Spells/WindGustSpell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Shiftable, ShiftableTile)

ShiftableTile::ShiftableTile(Level* level) : LevelDynamicTile(level), MovableGameObject() {
}

void ShiftableTile::init() {
	setSpriteOffset(sf::Vector2f(-1.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(TILE_SIZE) - 2.f, TILE_SIZE_F));
}

void ShiftableTile::loadAnimation(int skinNr) {
	m_isCollidable = true;

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shiftable));
	idleAnimation->addFrame(sf::IntRect(BORDER, BORDER + ((skinNr - 1) * (TILE_SIZE + 2 * BORDER)), TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* crumblingAnimation = new Animation();
	crumblingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_destructible));
	for (int i = 1; i < 5; i++) {
		crumblingAnimation->addFrame(sf::IntRect(
			BORDER + i * (2 * BORDER + TILE_SIZE),
			BORDER,
			TILE_SIZE,
			TILE_SIZE));
	}

	addAnimation(GameObjectState::Crumbling, crumblingAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void ShiftableTile::update(const sf::Time& frameTime) {
	if (m_state == GameObjectState::Crumbling) {
		updateTime(m_crumblingTime, frameTime);
		if (m_crumblingTime == sf::Time::Zero) {
			setDisposed();
		}
		MovableGameObject::update(frameTime);
		return;
	}
	setAcceleration(sf::Vector2f(m_pushAcceleration, GRAVITY_ACCELERATION));
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);
	m_pushAcceleration = 0.f;
}

void ShiftableTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::WindGust: {
		float pushAcceleration = dynamic_cast<WindGustSpell*>(spell)->getPushAcceleration();
		// determine the direction of the windgust by the position of its owner.
		if (spell->getOwner()->getPosition().x < getPosition().x) {
			m_pushAcceleration = pushAcceleration;
		}
		else {
			m_pushAcceleration = -pushAcceleration;
		}
		break;
	}
	default:
		break;
	}
}

GameObjectType ShiftableTile::getConfiguredType() const {
	return GameObjectType::_MovableTile;
}

void ShiftableTile::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.f) ? DAMPING_GROUND : DAMPING_AIR;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.f) dampingPerSec = 0.f;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

void ShiftableTile::updateRelativeVelocity(const sf::Time& frameTime) {
	if (m_movingParent == nullptr) return;
	sf::Vector2f oldPos = m_position;
	MovableGameObject::updateRelativeVelocity(frameTime);
	sf::Vector2f posDiff = m_position - oldPos;

	// check if we hit the other movable objects that do not have the same parent as us. we have precedence and shift other objects away.
	auto mainChars = m_screen->getObjects(GameObjectType::_LevelMainCharacter);
	auto enemies = m_screen->getObjects(GameObjectType::_Enemy);
	auto movableTiles = m_screen->getObjects(GameObjectType::_MovableTile);

	LevelMovableGameObject* mainChar = dynamic_cast<LevelMovableGameObject*>((*mainChars)[0]);
	if (mainChar->getMovingParent() != getMovingParent() && !mainChar->isDead()) {
		const sf::FloatRect& mainCharBB = *mainChar->getBoundingBox();
		if (epsIntersect(mainCharBB, m_boundingBox)) {
			mainChar->setPosition(mainChar->getPosition() + posDiff);
		}
	}

	for (auto enemy : *enemies) {
		LevelMovableGameObject* mob = dynamic_cast<LevelMovableGameObject*>(enemy);
		if (mob->getMovingParent() != getMovingParent() && !mob->isDead()) {
			const sf::FloatRect& mobBB = *mob->getBoundingBox();
			if (epsIntersect(mobBB, m_boundingBox)) {
				mob->setPosition(mob->getPosition() + posDiff);
			}
		}
	}

	for (auto movableTile : *movableTiles) {
		MovableGameObject* mob = dynamic_cast<MovableGameObject*>(movableTile);
		LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(movableTile);
		if (mob->getMovingParent() != getMovingParent() && tile->getDynamicTileID() != LevelDynamicTileID::Moving && tile->getIsCollidable()) {
			const sf::FloatRect& mobBB = *mob->getBoundingBox();
			if (epsIntersect(mobBB, m_boundingBox)) {
				mob->setPosition(mob->getPosition() + posDiff);
			}
		}
	}
}

void ShiftableTile::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::Vector2f oldPosition = getPosition();
	const sf::FloatRect& bb = *getBoundingBox();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	WorldCollisionQueryRecord rec;

	rec.excludedGameObject = this;
	rec.ignoreMobs = false;

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;

	// should we use strategy 2: try y direction first, then x direction?
	bool tryYfirst = false;
	rec.boundingBox = nextBoundingBoxX;
	rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
	if (m_level->collides(rec)) {
		if (std::abs(nextPosition.x - rec.safeLeft) > std::abs(getVelocity().x) + 10.f) {
			tryYfirst = true;
		}
	}

	if (!tryYfirst) {
		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
		bool collidesX = m_level->collides(rec);

		if (collidesX) {
			setAccelerationX(0.f);
			setVelocityX(0.f);
			setPositionX(rec.safeLeft);
			nextBoundingBoxY.left = rec.safeLeft;
		}
		else {
			nextBoundingBoxY.left = nextPosition.x;
		}

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
	else {
		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;

		bool collidesY = m_level->collides(rec);
		if (collidesY) {
			setMovingParent(rec.movingParent);
			setAccelerationY(0.f);
			setVelocityY(0.f);
			setPositionY(rec.safeTop);
			nextBoundingBoxX.top = rec.safeTop;
		}
		else {
			nextBoundingBoxX.top = nextPosition.y;
		}

		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;

		bool collidesX = m_level->collides(rec);
		if (collidesX) {
			setAccelerationX(0.f);
			setVelocityX(0.f);
			setPositionX(rec.safeLeft);
		}
	}

	if (dist(oldPosition, getPosition()) > TILE_SIZE_F / 2.f + norm(getVelocity())) {
		setPosition(oldPosition);
		setMovingParent(nullptr);
		setVelocity(sf::Vector2f(0.f, 0.f));
		setAcceleration(sf::Vector2f(0.f, 0.f));
		setState(GameObjectState::Crumbling);
		m_isCollidable = false;
	}
}