#include "Spells/ReturningProjectileSpell.h"

void ReturningProjectileSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);
}

void ReturningProjectileSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-19.f, 1.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_returningprojectile));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr - 1 * 30, 90, 30));

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
		float abs = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		dir = dir / abs * m_absVel;
		nextVel.x = dir.x;
		nextVel.y = dir.y;
	}
	else {
		nextVel.x = getVelocity().x;
		nextVel.y = getVelocity().y;
	}
}

void ReturningProjectileSpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	sf::Vector2f diff = nextPosition - getPosition();

	if (!m_data.isAlly) {
		// check collisions with allies
		checkCollisionsWithAllies(getBoundingBox());
	}
	else {
		// check collisions with enemies
		checkCollisionsWithEnemies(getBoundingBox());
	}

	MovableGameObject::update(frameTime);
	updateTime(m_data.activeDuration, frameTime);

	// check collisions with owner
	if (m_isReturning && m_mob->getBoundingBox()->intersects(*getBoundingBox())) {
		setDisposed();
		m_mob->setReady();
	}

	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
		m_mob->setReady();
	}

	if (!m_isReturning) {
		m_rangeLeft -= norm(diff);
		if (m_rangeLeft <= 0.f) {
			m_isReturning = true;
			m_absVel = std::sqrt(getVelocity().x * getVelocity().x + getVelocity().y * getVelocity().y);
		}
	}
	else {
		setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
	}
}

sf::Vector2f ReturningProjectileSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(5.f, -5.f);
}

void ReturningProjectileSpell::onOwnerDisposed() {
	setDisposed();
	m_mob = nullptr;
}


