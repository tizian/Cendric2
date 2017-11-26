#include "Level/DynamicTiles/ShiftableTile.h"
#include "Spells/Spell.h"
#include "Spells/WindGustSpell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Shiftable, ShiftableTile)

ShiftableTile::ShiftableTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen),
	LevelMovableTile(levelScreen) {
}

bool ShiftableTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(-1.f, 0.f));
	setPositionOffset(sf::Vector2f(1.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F - 2.f, TILE_SIZE_F));

	return true;
}

void ShiftableTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* crumblingAnimation = new Animation();
	crumblingAnimation->setSpriteSheet(tex);
	for (int i = 1; i < 5; i++) {
		crumblingAnimation->addFrame(sf::IntRect(i * TILE_SIZE, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}
	crumblingAnimation->setLooped(false);

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
	if (m_boundingBox.top + m_boundingBox.height > (m_level->getWorldRect().top + m_level->getWorldRect().height)) {
		setDisposed();
	}
}

void ShiftableTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::WindGust: {
		m_pushAcceleration = dynamic_cast<WindGustSpell*>(spell)->getPushAcceleration();
		break;
	}
	default:
		break;
	}
}

void ShiftableTile::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.f) ? DAMPING_GROUND : DAMPING_AIR;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.f) dampingPerSec = 0.f;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

std::string ShiftableTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_shiftable.png";
}


