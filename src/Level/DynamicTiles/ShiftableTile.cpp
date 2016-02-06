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

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void ShiftableTile::update(const sf::Time& frameTime) {
	setAcceleration(sf::Vector2f(m_pushAcceleration, GRAVITY_ACCELERATION));
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);
	m_pushAcceleration = 0.f;

	if (m_debugInfo) {
		m_debugInfo->setString("x: " + std::to_string(getPosition().x) + " y: " + std::to_string(getPosition().y));
		m_debugInfo->setPosition(getPosition() + sf::Vector2f(0.f, -30.f));
	}
}

void ShiftableTile::updateRelativeVelocity(const sf::Time& frameTime) {
	if (m_relativeVelocity.x == 0.f && m_relativeVelocity.y == 0.f) return;
	sf::Vector2f nextPos;
	nextPos.x = m_position.x + m_relativeVelocity.x * frameTime.asSeconds();
	nextPos.y = m_position.y + (m_relativeVelocity.y + 50.f) * frameTime.asSeconds();
	WorldCollisionQueryRecord rec;
	rec.excludedGameObject = this;
	rec.ignoreMobs = false;
	rec.boundingBox = *getBoundingBox();
	rec.boundingBox.left = nextPos.x;
	if (!m_level->collides(rec))
		setPositionX(nextPos.x);

	rec.boundingBox.top = nextPos.y;
	if (!m_level->collides(rec))
		setPositionY(nextPos.y);
}

void ShiftableTile::setDebugBoundingBox(const sf::Color &debugColor) {
	if (!g_resourceManager->getConfiguration().isDebugRendering) return;
	LevelDynamicTile::setDebugBoundingBox(debugColor);
	delete m_debugInfo;
	m_debugInfo = new BitmapText();
	m_debugInfo->setColor(sf::Color::Red);
}

void ShiftableTile::renderAfterForeground(sf::RenderTarget& target) {
	LevelDynamicTile::renderAfterForeground(target);
	if (m_debugInfo) {
		target.draw(*m_debugInfo);
	}
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
	return LevelDynamicTile::getConfiguredType();
}

void ShiftableTile::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.0f) ? DAMPING_GROUND : DAMPING_AIR;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

void ShiftableTile::checkCollisions(const sf::Vector2f& nextPosition) {
	const sf::FloatRect& bb = *getBoundingBox();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	WorldCollisionQueryRecord rec;
	rec.excludedGameObject = this;
	rec.ignoreMobs = false;

	bool isMovingDown = nextPosition.y > bb.top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != bb.left;
	bool isMovingRight = nextPosition.x > bb.left;

	// check for collision on x axis
	rec.boundingBox = nextBoundingBoxX;
	bool collidesX = isMovingX && m_level->collides(rec);

	if (collidesX) {
		setAccelerationX(0.f);
		setVelocityX(0.f);
		if (isMovingRight) {
			setPositionX(m_level->getNonCollidingLeft(rec));
		}
		else {
			setPositionX(m_level->getNonCollidingRight(rec));
		}
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}

	// check for collision on y axis
	rec.boundingBox = nextBoundingBoxY;
	rec.checkMovingPlatforms = isMovingDown;
	rec.gainedRelativeVelocity = sf::Vector2f(0.f, 0.f);
	bool collidesY = m_level->collides(rec);
	if (isMovingDown) setRelativeVelocity(rec.gainedRelativeVelocity);
	if (collidesY) {
		setAccelerationY(0.f);
		setVelocityY(0.0f);
		if (isMovingDown) {
			float bla = m_level->getNonCollidingTop(rec);
			setPositionY(bla);
		}
		else {
			setPositionY(m_level->getNonCollidingBottom(rec));
		}
	}
}