#include "Spells/LeechSpell.h"
#include "Level/LevelMainCharacter.h"

LeechSpell::LeechSpell() : Spell() {
}

void LeechSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_fear));
	spellAnimation->addFrame(sf::IntRect(0, 0, 30, 30));
	spellAnimation->addFrame(sf::IntRect(30, 0, 30, 30));

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
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);

	if (!m_isReturning) {
		checkCollisions(nextPosition);

		if (!m_data.isAlly) {
			// check collisions with allies
			if (m_mainChar->getBoundingBox()->intersects(*getBoundingBox())) {
				m_mainChar->onHit(this);
				m_isDisposed = false;
				m_isReturning = true;
			}
			for (auto& go : *m_enemies) {
				if (!go->isViewable()) continue;
				Enemy* enemy = dynamic_cast<Enemy*>(go);
				if (!enemy->isAlly()) continue;
				if (enemy->getBoundingBox()->intersects(*getBoundingBox())) {
					enemy->onHit(this);
					m_isDisposed = false;
					m_isReturning = true;
				}
			}
		}
		else {
			// check collisions with enemies
			for (auto& go : *m_enemies) {
				if (!go->isViewable()) continue;
				Enemy* enemy = dynamic_cast<Enemy*>(go);
				if (enemy->isAlly()) continue;
				if (enemy->getBoundingBox()->intersects(*getBoundingBox())) {
					enemy->onHit(this);
					m_isDisposed = false;
					m_isReturning = true;
				}
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
	GameObject::updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration.asMilliseconds() <= 0) {
		setDisposed();
	}
}
