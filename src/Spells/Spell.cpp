#include "Spells/Spell.h"
#include "Level/LevelMovableGameObject.h"
#include "Level/Enemy.h"
#include "Screens/LevelScreen.h"

using namespace std;

void Spell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	m_data = data;
	setBoundingBox(data.boundingBox);
	setDebugBoundingBox(COLOR_BAD);

	m_level = mob->getLevel();
	m_mob = mob;
	m_screen = mob->getScreen();
	m_enemies = m_screen->getObjects(GameObjectType::_Enemy);

	m_mainChar = dynamic_cast<LevelScreen*>(m_screen)->getMainCharacter();

	sf::Vector2f absolutePosition;
	calculatePositionAccordingToMob(absolutePosition);
	setPosition(absolutePosition);

	// if it is attached to mob, its velocity is ignored 
	if (data.attachedToMob || !data.needsTarget) {
		setVelocity(sf::Vector2f(0, 0));
		return;
	}

	sf::Vector2f trueDir = target - absolutePosition;
	// normalize dir
	float len = sqrt(trueDir.x * trueDir.x + trueDir.y * trueDir.y);
	trueDir.x = (len == 0) ? 0 : trueDir.x / len;
	trueDir.y = (len == 0) ? 0 : trueDir.y / len;

	sf::Vector2f direction = rotateVector(trueDir, data.divergenceAngle);

	if (getConfiguredRotateSprite()) {
		setSpriteRotation(atan2(direction.y, direction.x));
	}

	setVelocity(m_data.speed * direction);
}

void Spell::load(const SpellData& data, LevelDynamicTile* tile, const sf::Vector2f& target) {
	// very similar to Spell::load with a mob, but its using a LevelDynamicTile instead
	if (data.attachedToMob) {
		g_logger->logError("Spell", "Cannot load a spell that needs a mob with a tile as owner!");
		throw;
	}

	m_data = data;
	setBoundingBox(data.boundingBox);
	setDebugBoundingBox(COLOR_BAD);

	m_level = tile->getLevel();
	m_mob = nullptr;
	m_screen = tile->getScreen();
	m_enemies = m_screen->getObjects(GameObjectType::_Enemy);

	m_mainChar = dynamic_cast<LevelScreen*>(m_screen)->getMainCharacter();

	sf::Vector2f absolutePosition = tile->getCenter() - sf::Vector2f(data.boundingBox.width / 2.f, data.boundingBox.height / 2.f);
	setPosition(absolutePosition);

	sf::Vector2f trueDir = target - sf::Vector2f(data.boundingBox.width / 2.f, data.boundingBox.height / 2.f) - absolutePosition;
	// normalize dir
	float len = sqrt(trueDir.x * trueDir.x + trueDir.y * trueDir.y);
	trueDir.x = (len == 0) ? 0 : trueDir.x / len;
	trueDir.y = (len == 0) ? 0 : trueDir.y / len;

	sf::Vector2f direction = rotateVector(trueDir, data.divergenceAngle);

	if (getConfiguredRotateSprite()) {
		setSpriteRotation(atan2(direction.y, direction.x));
	}

	setVelocity(m_data.speed * direction);
}

void Spell::execOnHit(LevelMovableGameObject* target) {
	setDisposed();
	
	if (!m_data.isFearing && !m_data.isStunning) {
		return;
	}
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		if (enemy->getMentalStrength() >= m_data.strength) {
			return;
		}
	}

	if (m_data.isFearing)
		target->setFeared(m_data.duration);
	else if (m_data.isStunning)
		target->setStunned(m_data.duration);
}

void Spell::calculatePositionAccordingToMob(sf::Vector2f& position) const {
	if (m_mob == nullptr) {
		// owner could be dead and looted.
		return;
	}
	sf::Vector2f mobPosition(m_mob->getPosition().x + (m_mob->getBoundingBox()->width / 2), m_mob->getPosition().y);
	sf::Vector2f offset = getConfiguredPositionOffset() + m_mob->getConfiguredSpellOffset();
	if (!m_mob->isFacingRight())
		offset.x = -offset.x - getBoundingBox()->width;
	if (m_mob->isUpsideDown())
		offset.y = m_mob->getBoundingBox()->height - offset.y - getBoundingBox()->height;

	position.x = (mobPosition + offset).x;
	position.y = (mobPosition + offset).y;
}

void Spell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	if (m_data.attachedToMob) {
		calculatePositionAccordingToMob(nextPosition);
		setPosition(nextPosition);
	}
	else {
		calculateNextPosition(frameTime, nextPosition);
	}

	// check collisions with the collidable level
	if (!m_data.attachedToMob) {
		checkCollisions(nextPosition);
	}

	// check collisions with dynamic tiles
	if (m_data.isDynamicTileEffect) {
		sf::FloatRect tmp(nextPosition, sf::Vector2f(getBoundingBox()->width, getBoundingBox()->height));
		m_level->collideWithDynamicTiles(this, tmp);
	}
	
	if (!m_data.isAlly) {
		// check collisions with main char
		checkCollisionsWithAllies(getBoundingBox());
	}
	else {
		// check collisions with enemies
		checkCollisionsWithEnemies(getBoundingBox());
	}

	MovableGameObject::update(frameTime);
	updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}
}

sf::Vector2f Spell::getConfiguredPositionOffset() const {
	return sf::Vector2f(20.f, -20.f);
}

bool Spell::getConfiguredRotateSprite() const {
	return true;
}

void Spell::setViewable(bool value) {
	if (!value) {
		setDisposed();
	}
	MovableGameObject::setViewable(value);
}

void Spell::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);
	WorldCollisionQueryRecord rec;

	bool isMovingY = nextPosition.y != getBoundingBox()->top;
	bool isMovingX = nextPosition.x != getBoundingBox()->left;
	bool reflected = false;
	// check for collision on x axis
	rec.boundingBox = nextBoundingBoxX;
	if (isMovingX && m_level->collides(rec)) {
		if (m_data.reflectCount <= 0) {
			setDisposed();
			return;
		}
		else {
			reflected = true;
			setAcceleration(sf::Vector2f(0.f, 0.f));
			setVelocityX(-getVelocity().x);
			if (getConfiguredRotateSprite()) {
				setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
			}
		}
	}
	// check for collision on y axis
	rec.boundingBox = nextBoundingBoxY;
	if (isMovingY && m_level->collides(rec)) {
		if (m_data.reflectCount <= 0) {
			setDisposed();
			return;
		}
		else {
			reflected = true;
			setAcceleration(sf::Vector2f(0.f, 0.f));
			setVelocityY(-getVelocity().y);
			if (getConfiguredRotateSprite()) {
				setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
			}
		}
	}
	if (reflected) {
		m_data.reflectCount -= 1;
	}
}

void Spell::checkCollisionsWithAllies(const sf::FloatRect* boundingBox) {
	if (m_mainChar->getBoundingBox()->intersects(*boundingBox)) {
		m_mainChar->onHit(this);
	}
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(go);
		if (!enemy->isAlly()) continue;
		if (enemy->getBoundingBox()->intersects(*boundingBox)) {
			enemy->onHit(this);
		}
	}
}

void Spell::checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) {
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(go);
		if (enemy->isAlly()) continue;
		if (enemy->getBoundingBox()->intersects(*boundingBox)) {
			enemy->onHit(this);
		}
	}
}

const sf::Time& Spell::getActiveDuration() const {
	return m_data.activeDuration;
}

const sf::Time& Spell::getDuration() const {
	return m_data.duration;
}

GameObjectType Spell::getConfiguredType() const {
	return GameObjectType::_Spell;
}

DamageType Spell::getDamageType() const {
	return m_data.damageType;
}

SpellID Spell::getSpellID() const {
	return m_data.id;
}

SpellType Spell::getSpellType() const {
	return m_data.spellType;
}

const MovableGameObject* Spell::getOwner() const {
	return m_mob;
}

bool Spell::isAttachedToMob() const {
	return m_data.attachedToMob;
}

int Spell::getDamage() const {
	return m_data.damage;
}

int Spell::getDamagePerSecond() const {
	return m_data.damagePerSecond;
}

int Spell::getHeal() const {
	return m_data.heal;
}

sf::Vector2f Spell::rotateVector(const sf::Vector2f &vec, float angle) {
	float newX = vec.x * cos(angle) - vec.y * sin(angle);
	float newY = vec.x * sin(angle) + vec.y * cos(angle);
	return sf::Vector2f(newX, newY);
}

