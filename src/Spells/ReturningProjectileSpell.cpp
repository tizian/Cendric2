#include "Spells/ReturningProjectileSpell.h"
#include "Level/LevelMainCharacter.h"

void ReturningProjectileSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);
	m_absVel = norm(getVelocity());
}

void ReturningProjectileSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-30.f, -5.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * 30, 90, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);

	m_rangeLeft = data.range;
}

void ReturningProjectileSpell::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	if (m_isReturning && m_mob != nullptr) {
		// a returning projectile spell will chase its owner.
		sf::Vector2f dir = m_mob->getCenter() - getPosition();
		float abs = norm(dir);
		dir = dir / abs * m_absVel;
		nextVel.x = dir.x;
		nextVel.y = dir.y;
	}
	else {
		nextVel.x = getVelocity().x;
		nextVel.y = getVelocity().y;
	}
}

void ReturningProjectileSpell::checkCollisions(const sf::Vector2f& nextPosition) {
	if (m_isReturning) return;

	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);
	WorldCollisionQueryRecord rec;
	rec.collisionDirection = CollisionDirection::Down;

	bool isMovingY = nextPosition.y != getBoundingBox()->top;
	bool isMovingX = nextPosition.x != getBoundingBox()->left;
	bool reflected = false;
	// check for collision on x axis
	rec.boundingBox = nextBoundingBoxX;
	if (isMovingX && m_level->collides(rec)) {
		if (m_data.reflectCount <= 0) {
			m_isReturning = true;
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
			m_isReturning = true;
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

void ReturningProjectileSpell::update(const sf::Time& frameTime) {
	if (m_isDisposed) return;

	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	sf::Vector2f diff = nextPosition - getPosition();

	checkCollisions(nextPosition);

	if (m_isDamaging) {
		if (!m_data.isAlly) {
			// check collisions with allies
			if (checkCollisionsWithAllies(getBoundingBox())) {
				m_isDisposed = false;
				m_isReturning = true;
				m_isDamaging = false;
			}
		}
		else {
			// check collisions with enemies
			if (checkCollisionsWithEnemies(getBoundingBox())) {
				m_isDisposed = false;
				m_isReturning = true;
				m_isDamaging = false;
			}
		}
	}

	if (!m_isReturning) {
		m_rangeLeft -= norm(diff);
		if (m_rangeLeft <= 0.f) {
			m_isReturning = true;
		}
	}
	else {
		setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
		// check collisions with owner
		if (fastIntersect(*m_mob->getBoundingBox(), *getBoundingBox())) {
			setDisposed();
		}
	}

	MovableGameObject::update(frameTime);
	updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration.asMilliseconds() <= 0) {
		setDisposed();
	}
}

void ReturningProjectileSpell::onOwnerDisposed() {
	setDisposed();
	m_mob = nullptr;
}

void ReturningProjectileSpell::setDisposed() {
	Spell::setDisposed();
	m_mob->setReady();
}


