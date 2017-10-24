#include "Level/DynamicTiles/FallingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "Screens/WorldScreen.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Falling, FallingTile)

FallingTile::FallingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen),
	MovableGameObject() {
	setPositionOffset(sf::Vector2f(3.f, 3.f));
}

bool FallingTile::init(const LevelTileProperties& properties) {
	m_initialHeight = getPosition().y + getPositionOffset().y;
	setSpriteOffset(sf::Vector2f(-3.f, -3.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F - 6.f, TILE_SIZE_F - 6.f));
	m_tileState = FallingTileState::Idle;
	
	return true;
}

void FallingTile::loadAnimation(int skinNr) {
	m_isCollidable = false;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void FallingTile::onHit(LevelMovableGameObject* mob) {
	if (mob->getConfiguredType() == GameObjectType::_LevelMainCharacter)
		mob->setDead();
}

void FallingTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_tileState == FallingTileState::Waiting) {
		updateTime(m_waitingTime, frameTime);
		if (m_waitingTime == sf::Time::Zero) {
			m_tileState = FallingTileState::Returning;
		}
		return;
	}
	if (m_tileState == FallingTileState::Idle) {
		if (std::abs(m_mainChar->getPosition().x - getCenter().x) < AGGRO_DISTANCE) {
			m_tileState = FallingTileState::Falling;
		}
		return;
	}
	if (m_tileState == FallingTileState::Falling) {
		setAcceleration(sf::Vector2f(0.f, GRAVITY_ACCELERATION));
	}
	else {
		// state is  returning
		if (getPosition().y <= m_initialHeight) {
			setPositionY(m_initialHeight);
			m_tileState = FallingTileState::Idle;
			return;
		}
		setVelocity(sf::Vector2f(0.f, -RETURN_VELOCITY));
	}

	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);
	if (getVelocity().y == 0.f) {
		if (m_tileState == FallingTileState::Falling) {
			m_tileState = FallingTileState::Waiting;
			m_waitingTime = WAITING_TIME;
		}
		else if (m_tileState == FallingTileState::Returning) {
			m_tileState = FallingTileState::Idle;
		}
	}
}

void FallingTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Telekinesis: {
		m_tileState = FallingTileState::Falling;
		spell->setDisposed();
		break;
	}
	default:
		break;
	}
}

void FallingTile::checkCollisions(const sf::Vector2f& nextPosition) {
	const sf::FloatRect& bb = *getBoundingBox();
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	WorldCollisionQueryRecord rec;

	rec.excludedGameObject = this;

	bool isMovingDown = nextPosition.y > bb.top;

	// check for collision on y axis
	rec.boundingBox = nextBoundingBoxY;
	rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
	rec.movingParent = nullptr;
	rec.ignoreOnewayTiles = true;
	bool collidesY = m_level->collides(rec);
	if (collidesY) {
		for (auto& it : *m_level->getDynamicTiles()) {
			LevelDynamicTile* tile = dynamic_cast<LevelDynamicTile*>(it);
			if (tile == nullptr || tile->getDynamicTileID() != LevelDynamicTileID::Ice) continue;
			const sf::FloatRect& tileBB = *tile->getBoundingBox();
			if (fastIntersect(nextBoundingBoxY, tileBB)) {
				tile->setDisposed();
				return;
			}
		}

		setAccelerationY(0.f);
		setVelocityY(0.f);
		setPositionY(rec.safeTop);
	}
}

std::string FallingTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_falling.png";
}
