#include "Spells/LeechSpell.h"
#include "Level/LevelMainCharacter.h"
#include "Particles/ParticleSystem.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "GlobalResource.h"

LeechSpell::LeechSpell() : Spell() {
}

void LeechSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();

	m_absVel = std::sqrt(getVelocity().x * getVelocity().x + getVelocity().y * getVelocity().y);

	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 80.f, 0.3f);
	addComponent(new LightComponent(lightData, this));
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
	if (m_isDisposed) return;

	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);

	if (!m_isReturning) {
		checkCollisions(nextPosition);

		if (!m_data.isAlly) {
			// check collisions with allies
			if (m_mainChar->getBoundingBox()->intersects(*getBoundingBox())) {
				m_mainChar->onHit(this);
				goReturn();
			}
			for (auto& go : *m_enemies) {
				if (!go->isViewable()) continue;
				Enemy* enemy = dynamic_cast<Enemy*>(go);
				if (!enemy->isAlly() || enemy->isDead()) continue;
				if (enemy->getBoundingBox()->intersects(*getBoundingBox())) {
					enemy->onHit(this);
					goReturn();
				}
			}
		}
		else {
			// check collisions with enemies
			for (auto& go : *m_enemies) {
				if (!go->isViewable()) continue;
				Enemy* enemy = dynamic_cast<Enemy*>(go);
				if (enemy->isAlly() || enemy->isDead()) continue;
				if (enemy->getBoundingBox()->intersects(*getBoundingBox())) {
					enemy->onHit(this);
					goReturn();
				}
			}
		}
	}
	else {
		// check collisions with owner
		if (m_mob->getBoundingBox()->intersects(*getBoundingBox())) {
			m_mob->addHeal(static_cast<int>(std::floor(getDamage() * 0.5f)), false, isCritical());
			setDisposed();
		}
	}

	MovableGameObject::update(frameTime);
	updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration.asMilliseconds() <= 0) {
		setDisposed();
	}
}

void LeechSpell::goReturn() {
	m_isDisposed = false;
	m_isReturning = true;
	m_colorGenerator->minStartCol = sf::Color(31, 86, 28, 100);
	m_colorGenerator->maxStartCol = sf::Color(44, 132, 57, 150);
	m_colorGenerator->minEndCol = sf::Color(0, 255, 0, 0);
	m_colorGenerator->maxEndCol = sf::Color(200, 255, 200, 0);
}

void LeechSpell::onOwnerDisposed() {
	setDisposed();
	m_mob = nullptr;
}

void LeechSpell::loadComponents() {
	ParticleComponentData data;
	data.particleCount = 50;
	data.emitRate = 50.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;

	// Generators
	auto spawner = new particles::BoxSpawner();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 20.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 5.f;
	sizeGen->maxEndSize = 10.f;
	data.sizeGen = sizeGen;

	m_colorGenerator = new particles::ColorGenerator();
	m_colorGenerator->minStartCol = sf::Color(86, 31, 28, 100);
	m_colorGenerator->maxStartCol = sf::Color(132, 44, 57, 150);
	m_colorGenerator->minEndCol = sf::Color(255, 0, 0, 0);
	m_colorGenerator->maxEndCol = sf::Color(255, 200, 200, 0);
	data.colorGen = m_colorGenerator;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 5.f;
	velGen->maxStartSpeed = 10.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(pc);
}
