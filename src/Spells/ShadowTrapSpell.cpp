#include "Spells/ShadowTrapSpell.h"

ShadowTrapSpell::ShadowTrapSpell() : Spell() {
}

void ShadowTrapSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -20.f));
	m_stunDuration = sf::seconds(2.f);

	Animation* spellAnimation = new Animation(sf::seconds(10.f));
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(bean.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, 0, 45, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);

	Spell::load(bean, mob, target);
}

void ShadowTrapSpell::execOnHit(LevelMovableGameObject *target) {
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
		target->setFeared(m_stunDuration);
	else if (m_data.isStunning)
		target->setStunned(m_stunDuration);
}

bool ShadowTrapSpell::getConfiguredRotateSprite() const {
	return false;
}

float ShadowTrapSpell::getConfiguredMaxVelocityYDown() const {
	return 300.f;
}

void ShadowTrapSpell::update(const sf::Time& frameTime) {
	// gravity
	setAccelerationY(1000.f);
	Spell::update(frameTime);
}

void ShadowTrapSpell::checkCollisions(const sf::Vector2f& nextPosition) {
	// a shadow trap is influenced by gravity and only falls in y direction,
	// which simplificates the collisions.
	WorldCollisionQueryRecord rec;
	rec.boundingBox = sf::FloatRect(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	// check for collision on y axis down
	rec.collisionDirection = CollisionDirection::Down;
	bool collidesY = m_level->collides(rec);
	setMovingParent(rec.movingParent);
	if (collidesY) {
		setAccelerationY(0.f);
		setVelocityY(0.f);
		setPositionY(rec.safeTop);
	}
}