#include "Level/DynamicTiles/BoatTile.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Spells/WindGustSpell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Boat, BoatTile)

BoatTile::BoatTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen),
	LevelMovableTile(levelScreen) {
	m_movingParent = this;

	m_spellVec = levelScreen->getObjects(_Spell);
	m_isInitialized = false;
}

BoatTile::~BoatTile() {
}

bool BoatTile::init(const LevelTileProperties& properties) {
	m_isCollidable = true;
	m_isStrictlyCollidable = true;

	m_newIsFacingRight = !contains(properties, std::string("left"));

	setPositionOffset(sf::Vector2f(0.f, 3.5f * TILE_SIZE_F));
	setSpriteOffset(sf::Vector2f(0.f, -3.5f * TILE_SIZE_F));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 3 * TILE_SIZE_F, TILE_SIZE_F));
	m_sailBoundingBox = sf::FloatRect(0.f, 0.f, 3.f * TILE_SIZE_F, 3.f * TILE_SIZE_F);
	m_sailDebugBox = sf::RectangleShape(sf::Vector2f(m_sailBoundingBox.width, m_sailBoundingBox.height));
	m_sailDebugBox.setOutlineThickness(1.f);
	m_sailDebugBox.setFillColor(COLOR_TRANSPARENT);
	m_sailDebugBox.setOutlineColor(COLOR_GOOD);

	return true;
}

void BoatTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	int width = 3 * TILE_SIZE;
	int height = 5 * TILE_SIZE;

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * height, width, height));
	idleAnimation->setLooped(false);
	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* activatedAnimation = new Animation();
	activatedAnimation->setSpriteSheet(tex);
	activatedAnimation->addFrame(sf::IntRect(width, skinNr * height, width, height));
	activatedAnimation->setLooped(false);
	addAnimation(GameObjectState::Active, activatedAnimation);

	Animation* brokenAnimation = new Animation();
	brokenAnimation->setSpriteSheet(tex);
	brokenAnimation->addFrame(sf::IntRect(2 * width, skinNr * height, width, height));
	brokenAnimation->addFrame(sf::IntRect(3 * width, skinNr * height, width, height));
	brokenAnimation->setLooped(false);
	addAnimation(GameObjectState::Broken, brokenAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);

	m_isFacingRight = true;
	m_newState = GameObjectState::Idle;
}

void BoatTile::destroy() {
	m_newState = GameObjectState::Broken;
}

void BoatTile::update(const sf::Time& frameTime) {
	if (!m_isInitialized) {
		for (auto go : *m_screen->getObjects(_DynamicTile)) {
			if (auto dyn = dynamic_cast<LevelDynamicTile*>(go)) {
				auto id = dyn->getDynamicTileID();
				if (id == LevelDynamicTileID::Falling || id == LevelDynamicTileID::Damaging) {
					m_dynamicTileVec.push_back(dyn);
				}
			}
		}
		m_isInitialized = true;
	}

	// update animation
	if (m_acceleration.x != 0.f) {
		m_newIsFacingRight = m_acceleration.x > 0.f;
	}
	if (m_state != m_newState || m_newIsFacingRight != m_isFacingRight) {
		m_isFacingRight = m_newIsFacingRight;
		setState(m_newState, false);
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}

	if (m_state == GameObjectState::Broken) {
		setAcceleration(sf::Vector2f(0.f, GRAVITY_ACCELERATION));
	}
	else {
		checkForWind();
		checkForEnemies();
	}

	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);

	// update relative vel
	m_relativeVelocity = m_velocity;
}	

void BoatTile::updateRelativeVelocity(const sf::Time& frameTime) {
	// nop
}

void BoatTile::renderAfterForeground(sf::RenderTarget& target) {
	if (!m_isDebugRendering) return;
	LevelMovableTile::renderAfterForeground(target);
	target.draw(m_sailDebugBox);
}

void BoatTile::setPosition(const sf::Vector2f& position) {
	LevelMovableTile::setPosition(position);
	m_sailBoundingBox.left = position.x;
	m_sailBoundingBox.top = position.y - TILE_SIZE_F * 3.f;
	m_sailDebugBox.setPosition(sf::Vector2f(m_sailBoundingBox.left, m_sailBoundingBox.top));
}

void BoatTile::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - DAMPING, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

void BoatTile::onHit(Spell* spell) {
	// nop
}

std::string BoatTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_boat.png";
}

void BoatTile::checkForWind() {
	float pushAcceleration = 0.f;
	for (auto const go : *m_spellVec) {
		if (auto spell = dynamic_cast<WindGustSpell*>(go)) {
			if (fastIntersect(*spell->getBoundingBox(), m_sailBoundingBox)) {
				setAcceleration(sf::Vector2f(spell->getPushAcceleration(), 0.f));
				m_newState = GameObjectState::Active;
				return;
			}
		}
	}
	setAcceleration(sf::Vector2f(0.f, 0.f));
	m_newState = GameObjectState::Idle;
}

void BoatTile::checkForEnemies() {
	for (auto const go : m_dynamicTileVec) {
		if (fastIntersect(*go->getBoundingBox(), *getBoundingBox())) {
			destroy();
			return;
		}
	}
}
