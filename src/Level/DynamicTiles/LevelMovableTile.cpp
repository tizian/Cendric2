#include "Level/DynamicTiles/LevelMovableTile.h"
#include "Level/DynamicTiles/MovingTile.h"
#include "Level/Level.h"

LevelMovableTile::LevelMovableTile(Level* level) : LevelDynamicTile(level), MovableGameObject() {
}

GameObjectType LevelMovableTile::getConfiguredType() const {
	return GameObjectType::_MovableTile;
}

void LevelMovableTile::updateRelativeVelocity(const sf::Time& frameTime) {
	if (m_movingParent == nullptr) return;
	sf::Vector2f oldPos = getPosition();
	MovableGameObject::updateRelativeVelocity(frameTime);
	sf::Vector2f posDiff = getPosition() - oldPos;

	sf::FloatRect newBoundingBoxX = m_boundingBox;
	newBoundingBoxX.top -= posDiff.y;

	sf::FloatRect newBoundingBoxY = m_boundingBox;
	newBoundingBoxY.left -= posDiff.x;

	// check if we hit the other movable objects that do not have the same parent as us. we have precedence and shift other objects away.
	auto mainChars = m_screen->getObjects(GameObjectType::_LevelMainCharacter);
	auto enemies = m_screen->getObjects(GameObjectType::_Enemy);
	auto movableTiles = m_screen->getObjects(GameObjectType::_MovableTile);

	LevelMovableGameObject* mainChar = dynamic_cast<LevelMovableGameObject*>((*mainChars)[0]);
	if (mainChar->getMovingParent() != getMovingParent() && !mainChar->isDead()) {
		const sf::FloatRect& mainCharBB = *mainChar->getBoundingBox();
		if (epsIntersect(mainCharBB, newBoundingBoxX)) {
			mainChar->setPositionX(mainChar->getPosition().x + posDiff.x);
			mainChar->lockRelativeVelocityX();
		}
		if (epsIntersect(mainCharBB, newBoundingBoxY)) {
			mainChar->setPositionY(mainChar->getPosition().y + posDiff.y);
			mainChar->lockRelativeVelocityY();
		}
	}

	for (auto enemy : *enemies) {
		LevelMovableGameObject* mob = dynamic_cast<LevelMovableGameObject*>(enemy);
		if (mob->getMovingParent() != getMovingParent() && !mob->isDead()) {
			const sf::FloatRect& mobBB = *mob->getBoundingBox();
			if (epsIntersect(mobBB, newBoundingBoxX)) {
				mob->setPositionX(mob->getPosition().x + posDiff.x);
				mob->lockRelativeVelocityX();
			}
			if (epsIntersect(mobBB, newBoundingBoxY)) {
				mob->setPositionY(mob->getPosition().y + posDiff.y);
				mob->lockRelativeVelocityY();
			}
		}
	}

	for (auto movableTile : *movableTiles) {
		LevelMovableTile* tile = dynamic_cast<LevelMovableTile*>(movableTile);
		if (tile->getMovingParent() != getMovingParent() && tile->getDynamicTileID() != LevelDynamicTileID::Moving && tile->getIsCollidable()) {
			const sf::FloatRect& mobBB = *tile->getBoundingBox();
			if (epsIntersect(mobBB, newBoundingBoxX)) {
				tile->setPositionX(tile->getPosition().x + posDiff.x);
				tile->lockRelativeVelocityX();
			}
			if (epsIntersect(mobBB, newBoundingBoxY)) {
				tile->setPositionY(tile->getPosition().y + posDiff.y);
				tile->lockRelativeVelocityY();
			}
		}
	}
}

void LevelMovableTile::checkCollisions(const sf::Vector2f& nextPosition) {
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

	// check for wrong parent
	if (MovingTile* mt = getMovingParent()) {
		if (mt->getBoundingBox()->top + Epsilon < getBoundingBox()->top + getBoundingBox()->width) {
			setMovingParent(nullptr);
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