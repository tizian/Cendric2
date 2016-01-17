#include "Spells/AureolaSpell.h"

AureolaSpell::AureolaSpell() : Spell() {
}

void AureolaSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation(sf::seconds(0.3f));
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_aureola));
	spellAnimation.addFrame(sf::IntRect(0, 0, 40, 40));
	spellAnimation.addFrame(sf::IntRect(40, 0, 40, 40));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	m_rangeLeft = bean.range;

	Spell::load(bean, mob, target);
}

void AureolaSpell::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	if (m_isReturning) {
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
	calculateNextPosition(frameTime, m_nextPosition);
	sf::Vector2f div = m_nextPosition - getPosition();

	// check collisions with main char
	if (m_ownerType != GameObjectType::_LevelMainCharacter) {
		checkCollisionsWithMainChar(getBoundingBox());
	}
	// check collisions with enemies
	checkCollisionsWithEnemies(getBoundingBox());
	// check collisions with owner
	if (m_isReturning && m_mob->getBoundingBox()->intersects(*getBoundingBox())) {
		m_mob->addHeal(getHeal());
		setDisposed();
	}
	MovableGameObject::update(frameTime);
	GameObject::updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration.asMilliseconds() <= 0) {
		setDisposed();
	}

	if (!m_isReturning) {
		m_rangeLeft -= std::sqrt(div.x * div.x + div.y * div.y);
		if (m_rangeLeft <= 0.f) {
			m_isReturning = true;
			m_absVel = std::sqrt(getVelocity().x * getVelocity().x + getVelocity().y * getVelocity().y);
		}
	}
	else {
		setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
	}
}

sf::Vector2f AureolaSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(5.f, -5.f);
}
