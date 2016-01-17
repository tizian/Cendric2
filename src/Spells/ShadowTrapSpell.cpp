#include "Spells/ShadowTrapSpell.h"

ShadowTrapSpell::ShadowTrapSpell(int strength) : Spell() {
	m_strength = strength;
}

void ShadowTrapSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(-10.f, -20.f));
	m_stunDuration = sf::seconds(2.f);

	Animation spellAnimation(sf::seconds(10.f));
	spellAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_spell_shadowtrap));
	spellAnimation.addFrame(sf::IntRect(0, 0, 45, 30));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);

	Spell::load(bean, mob, target);
}

void ShadowTrapSpell::execOnHit(LevelMovableGameObject *target) {
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		if (enemy->getMentalStrength() <= m_strength) {
			enemy->setStunned(m_stunDuration);
			setDisposed();
		}
	}
	setDisposed();
	// main character can't be stunned yet.
}

bool ShadowTrapSpell::getConfiguredRotateSprite() const {
	return false;
}

float ShadowTrapSpell::getConfiguredMaxVelocityYDown() const {
	return 300.f;
}

sf::Vector2f ShadowTrapSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(10.f, 10.f);
}

void ShadowTrapSpell::update(const sf::Time& frameTime) {
	// gravity
	setAccelerationY(1000.f);
	Spell::update(frameTime);
}

void ShadowTrapSpell::checkCollisions(const sf::Vector2f& nextPosition) {
	// a shadow trap is influenced by gravity and only falls in y direction,
	// which simplificates the collisions.
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	// check for collision on y axis down
	bool collidesY = m_level->collides(nextBoundingBoxY);
	if (collidesY) {
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
		setPositionY(m_level->getGround(nextBoundingBoxY));
	}
}