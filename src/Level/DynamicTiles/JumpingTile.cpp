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
	m_tileState = JumpingTileState::Idle;
}

void JumpingTile::loadAnimation(int skinNr) {
	m_isCollidable = false;

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_falling));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void JumpingTile::onHit(LevelMovableGameObject* mob) {
	if (mob->getConfiguredType() == GameObjectType::_LevelMainCharacter)
		mob->setDead();
}

void JumpingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_tileState == JumpingTileState::Waiting) {
		updateTime(m_waitingTime, frameTime);
		if (m_waitingTime == sf::Time::Zero) {
			m_tileState = JumpingTileState::Returning;
		}
		return;
	}
	if (m_tileState == JumpingTileState::Idle) {
		if (std::abs(m_mainChar->getPosition().x - getCenter().x) < AGGRO_DISTANCE) {
			m_tileState = JumpingTileState::Falling;
		}
		return;
	}
	if (m_tileState == JumpingTileState::Falling) {
		setAcceleration(sf::Vector2f(0.f, GRAVITY_ACCELERATION));
	}
	else {
		setVelocity(sf::Vector2f(0.f, -RETURN_VELOCITY));
	}

	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);
	if (getVelocity().y == 0.f) {
		if (m_tileState == JumpingTileState::Falling) {
			m_tileState = JumpingTileState::Waiting;
			m_waitingTime = WAITING_TIME;
		}
		else if (m_tileState == JumpingTileState::Returning) {
			m_tileState = JumpingTileState::Idle;
		}
	}
}

void JumpingTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Telekinesis: {
		m_tileState = JumpingTileState::Falling;
		spell->setDisposed();
		break;
	}
	default:
		break;
	}
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


