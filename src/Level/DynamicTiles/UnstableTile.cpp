#include "Level/DynamicTiles/UnstableTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Unstable, UnstableTile)

const sf::Time UnstableTile::CRITICAL_TIME = sf::seconds(0.6f);

UnstableTile::UnstableTile(LevelScreen* levelScreen) :
    LevelDynamicTile(levelScreen),
    LevelMovableTile(levelScreen) {
	m_criticalTime = CRITICAL_TIME;
}

void UnstableTile::init() {
	setSpriteOffset(sf::Vector2f(-1.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(TILE_SIZE) - 2.f, TILE_SIZE_F));
}

void UnstableTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(BORDER, BORDER + ((skinNr - 1) * (TILE_SIZE + 2 * BORDER)), TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* tremblingAnimation = new Animation(sf::seconds(0.1f));
	tremblingAnimation->setSpriteSheet(tex);
	tremblingAnimation->addFrame(sf::IntRect(BORDER + 1 * (2 * BORDER + TILE_SIZE), BORDER + ((skinNr - 1) * (TILE_SIZE + 2 * BORDER)), TILE_SIZE, TILE_SIZE));
	tremblingAnimation->addFrame(sf::IntRect(BORDER + 2 * (2 * BORDER + TILE_SIZE), BORDER + ((skinNr - 1) * (TILE_SIZE + 2 * BORDER)), TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Trembling, tremblingAnimation);

	Animation* crumblingAnimation = new Animation();
	crumblingAnimation->setSpriteSheet(tex);
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

void UnstableTile::update(const sf::Time& frameTime) {
	if (m_state == GameObjectState::Crumbling) {
		updateTime(m_crumblingTime, frameTime);
		if (m_crumblingTime == sf::Time::Zero) {
			setDisposed();
		}
	}
	else if (m_isCritical) {
		updateTime(m_criticalTime, frameTime);
		if (m_criticalTime == sf::Time::Zero) {
			m_isFalling = true;
			setState(GameObjectState::Idle);
		}
	}
	else if (m_isFalling) {
		setAcceleration(sf::Vector2f(0.f, GRAVITY_ACCELERATION));
		sf::Vector2f nextPosition;
		calculateNextPosition(frameTime, nextPosition);
		checkCollisions(nextPosition);
	}
	MovableGameObject::update(frameTime);
	if (m_isCritical && !m_wasCritical) {
		m_criticalTime = CRITICAL_TIME;
		m_isCritical = false;
		setState(GameObjectState::Idle);
	}
	m_wasCritical = false;
	if (m_boundingBox.top + m_boundingBox.height > (m_level->getWorldRect().top + m_level->getWorldRect().height)) {
		setDisposed();
	}
}

void UnstableTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::WindGust:
	case SpellID::Chop:
		m_isFalling = true;
		break;
	case SpellID::Projectile:
	case SpellID::Telekinesis:
		m_isFalling = true;
		spell->setDisposed();
		break;
	default:
		break;
	}
}

void UnstableTile::onHit(LevelMovableGameObject* mob) {
	if (m_isFalling || m_state == GameObjectState::Crumbling) return;
	if (mob->getConfiguredType() != GameObjectType::_LevelMainCharacter) return;
	m_wasCritical = true;
	if (!m_isCritical) {
		m_isCritical = true;
		setState(GameObjectState::Trembling);
	}
}

void UnstableTile::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.f) ? DAMPING_GROUND : DAMPING_AIR;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.f) dampingPerSec = 0.f;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

std::string UnstableTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_unstable.png";
}


