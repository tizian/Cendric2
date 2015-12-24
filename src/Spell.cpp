#include "Spell.h"
#include "Level/LevelMovableGameObject.h"
#include "Level/LevelMainCharacter.h"

using namespace std;

void Spell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	m_duration = data.duration;
	m_damage = data.damage;
	m_heal = data.heal;
	m_damageType = data.damageType;
	m_reflectCount = data.reflectCount;
	m_speed = data.startVelocity;
	m_id = data.id;
	m_range = data.range;
	m_needsTarget = data.needsTarget;
	setBoundingBox(data.boundingBox);
	setDebugBoundingBox(sf::Color::Red);

	m_level = mob->getLevel();
	m_mob = mob;
	m_ownerType = mob->getConfiguredType();
	m_screen = mob->getScreen();
	m_enemies = m_screen->getObjects(GameObjectType::_Enemy);
	m_mainChar = dynamic_cast<LevelMainCharacter*>(m_screen->getObjects(GameObjectType::_MainCharacter)->at(0));

	if (m_mainChar == nullptr) {
		g_logger->logError("Spell", "Could not find main character of game screen");
		return;
	}

	sf::Vector2f absolutePosition;
	calculatePositionAccordingToMob(absolutePosition);
	setPosition(absolutePosition);

	// if the spell doesn't need a target, it is attached to main char and velocity is ignored 
	if (!data.needsTarget) {
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

	setVelocity(m_speed * direction);
}

void Spell::execOnHit(LevelMovableGameObject *target) {
	// nop
}

void Spell::calculatePositionAccordingToMob(sf::Vector2f& position) const {
	if (m_mob == nullptr) {
		// owner could be dead and looted.
		return;
	}
	sf::Vector2f mobPosition(m_mob->getPosition().x + (m_mob->getBoundingBox()->width / 2), m_mob->getPosition().y);
	sf::Vector2f offset = getConfiguredPositionOffset() + m_mob->getConfiguredSpellOffset();
	if (!m_mob->getIsFacingRight())
		offset.x = -offset.x - getBoundingBox()->width;
	if (m_mob->getIsUpsideDown())
		offset.y = m_mob->getBoundingBox()->height - offset.y - getBoundingBox()->height;

	position.x = (mobPosition + offset).x;
	position.y = (mobPosition + offset).y;
}

void Spell::update(const sf::Time& frameTime) {
	if (!m_needsTarget) {
		calculatePositionAccordingToMob(m_nextPosition);
		setPosition(m_nextPosition);
	}
	else {
		calculateNextPosition(frameTime, m_nextPosition);
	}

	checkCollisions(m_nextPosition);
	// check collisions with dynamic tiles
	sf::FloatRect tmp(m_nextPosition, sf::Vector2f(getBoundingBox()->width, getBoundingBox()->height));
	m_level->collideWithDynamicTiles(this, &tmp);
	// check collisions with main char
	if (m_ownerType != GameObjectType::_MainCharacter) {
		checkCollisionsWithMainChar(getBoundingBox());
	}
	// check collisions with enemies
	checkCollisionsWithEnemies(getBoundingBox());
	MovableGameObject::update(frameTime);

	GameObject::updateTime(m_duration, frameTime);

	if (m_duration == sf::Time::Zero) {
		setDisposed();
	}
}

const sf::Vector2f Spell::getConfiguredPositionOffset() const {
	return sf::Vector2f(20.f, 0.f);
}

bool Spell::getConfiguredRotateSprite() const {
	return true;
}

void Spell::setViewable(bool value) {
	if (!value) {
		setDisposed();
	}
}

void Spell::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingY = nextPosition.y != getBoundingBox()->top;
	bool isMovingX = nextPosition.x != getBoundingBox()->left;
	bool reflected = false;
	// check for collision on x axis
	if (isMovingX && m_level->collidesX(nextBoundingBoxX)) {
		if (m_reflectCount <= 0) {
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
	if (isMovingY && m_level->collidesY(nextBoundingBoxY)) {
		if (m_reflectCount <= 0) {
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
		m_reflectCount -= 1;
	}
}

void Spell::checkCollisionsWithMainChar(const sf::FloatRect* boundingBox) {
	if (m_mainChar->getBoundingBox()->intersects(*boundingBox)) {
		m_mainChar->onHit(this);
	}
}

void Spell::checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) {
	for (std::vector<GameObject*>::iterator it = m_enemies->begin(); it != m_enemies->end(); ++it) {
		if (!(*it)->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>((*it));
		if (enemy != nullptr && (enemy->getBoundingBox()->intersects(*boundingBox))) {
			enemy->onHit(this);
		}
	}
}

const sf::Time& Spell::getDuration() const {
	return m_duration;
}

GameObjectType Spell::getConfiguredType() const {
	return GameObjectType::_Spell;
}

DamageType Spell::getDamageType() const {
	return m_damageType;
}

SpellID Spell::getSpellID() const {
	return m_id;
}

const MovableGameObject* Spell::getOwner() const {
	return m_mob;
}

int Spell::getDamage() const {
	return m_damage;
}

int Spell::getHeal() const {
	return m_heal;
}

sf::Vector2f Spell::rotateVector(const sf::Vector2f &vec, float angle) {
	float newX = vec.x * cos(angle) - vec.y * sin(angle);
	float newY = vec.x * sin(angle) + vec.y * cos(angle);
	return sf::Vector2f(newX, newY);
}

