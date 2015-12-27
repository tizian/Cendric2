#include "Spells/LeechSpell.h"
#include "Level/LevelMainCharacter.h"

LeechSpell::LeechSpell() : Spell() {
}

void LeechSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation spellAnimation;
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fear));
	spellAnimation.addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation.addFrame(sf::IntRect(30, 0, 30, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	Spell::load(bean, mob, target);

	m_absVel = std::sqrt(getVelocity().x * getVelocity().x + getVelocity().y * getVelocity().y);
}

void LeechSpell::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	if (m_isReturning) {
		// a fear spell will chase its owner.
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

void LeechSpell::update(const sf::Time& frameTime) {
	calculateNextPosition(frameTime, m_nextPosition);

	if (!m_isReturning) {
		checkCollisions(m_nextPosition);
		// check collisions with main char
		if (m_ownerType != GameObjectType::_LevelMainCharacter) {
			if (m_mainChar->getBoundingBox()->intersects(*getBoundingBox())) {
				m_mainChar->onHit(this);
				m_isDisposed = false;
				m_isReturning = true;
			}
		}
		// check collisions with enemies
		for (std::vector<GameObject*>::iterator it = m_enemies->begin(); it != m_enemies->end(); ++it) {
			Enemy* enemy = dynamic_cast<Enemy*>((*it));
			if (enemy != nullptr && (enemy->getBoundingBox()->intersects(*getBoundingBox()))) {
				enemy->onHit(this);
				m_isDisposed = false;
				m_isReturning = true;
			}
		}
	}
	else {
		setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
		// check collisions with owner
		if (m_mob->getBoundingBox()->intersects(*getBoundingBox())) {
			m_mob->addHeal(getDamage());
			setDisposed();
		}
	}


	MovableGameObject::update(frameTime);

	m_duration = m_duration - frameTime;

	if (m_duration.asMilliseconds() <= 0) {
		setDisposed();
	}
}
