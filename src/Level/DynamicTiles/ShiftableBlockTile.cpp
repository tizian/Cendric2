#include "Level/DynamicTiles/ShiftableBlockTile.h"
#include "Spell.h"

ShiftableBlockTile::ShiftableBlockTile(Level* level) : LevelDynamicTile(level), MovableGameObject() {
}

void ShiftableBlockTile::init() {
	setSpriteOffset(sf::Vector2f(-1.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x) - 2.f, static_cast<float>(m_tileSize.y)));
}

void ShiftableBlockTile::load(int skinNr) {
	m_isCollidable = true;

	Animation idleAnimation(sf::seconds(10.f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shiftableblock));
	idleAnimation.addFrame(sf::IntRect(BORDER, BORDER + ((skinNr - 1) * (m_tileSize.x + 2 * BORDER)), m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void ShiftableBlockTile::update(const sf::Time& frameTime) {
	setAcceleration(sf::Vector2f(m_pushAcceleration, GRAVITY_ACCELERATION));
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
	m_pushAcceleration = 0.f;
}

void ShiftableBlockTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::WindGust:
		// determine the direction of the windgust by the position of its owner.
		if (spell->getOwner()->getPosition().x < getPosition().x) {
			m_pushAcceleration = PUSH_ACCELERATION_X;
		}
		else {
			m_pushAcceleration = -PUSH_ACCELERATION_X;
		}
		break;
	default:
		break;
	}
}

GameObjectType ShiftableBlockTile::getConfiguredType() const {
	return LevelDynamicTile::getConfiguredType();
}

void ShiftableBlockTile::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.0f) ? DAMPING_GROUND : DAMPING_AIR;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

void ShiftableBlockTile::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	if (isMovingX && m_level->collides(nextBoundingBoxX, this)) {
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}

	// check for collision on y axis
	bool collidesY = m_level->collides(nextBoundingBoxY, this);
	if (!isMovingDown && collidesY) {
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
	}
	else if (isMovingDown && collidesY) {
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
		setPositionY(m_level->getGround(nextBoundingBoxY));
	}
}