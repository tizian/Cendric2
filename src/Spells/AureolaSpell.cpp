#include "Spells/AureolaSpell.h"
#include "GameObjectComponents/LightComponent.h"

void AureolaSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation* spellAnimation = new Animation(sf::seconds(0.3f));
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, bean.skinNr * 40, 40, 40));
	spellAnimation->addFrame(sf::IntRect(40, bean.skinNr * 40, 40, 40));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	m_rangeLeft = bean.range;

	Spell::load(bean, mob, target);
	LightData lightData(sf::Vector2f(), 80.f, 0.4f);
	addComponent(new LightComponent(lightData, this));
}

void AureolaSpell::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	if (m_isReturning && m_mob != nullptr) {
		// an aureola spell will chase its owner.
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

void AureolaSpell::update(const sf::Time& frameTime) {
	if (m_isDisposed) return;

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
	if (m_isReturning && fastIntersect(*m_mob->getBoundingBox(), *getBoundingBox())) {
		m_mob->addHeal(getHeal(), false, isCritical());
		setDisposed();
	}

	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}

	if (!m_isReturning) {
		m_rangeLeft -= norm(diff);
		if (m_rangeLeft <= 0.f) {
			m_isReturning = true;
			m_absVel = norm(getVelocity());
		}
	}
	else {
		setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
	}
}

void AureolaSpell::onOwnerDisposed() {
	setDisposed();
	m_mob = nullptr;
}



