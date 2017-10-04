#include "Level/Enemies/RoyalguardBoss.h"
#include "Level/Level.h"

RoyalguardBoss::RoyalguardBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isAlwaysUpdate = true;
	m_isBoss = true;
}

void RoyalguardBoss::notifyOtherDeath(const sf::Vector2f& newPos) {
	if (m_isDead) return;
	setPosition(newPos);
	setStunned(sf::seconds(5.f));
}

void RoyalguardBoss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	clearReward();

	WorldCollisionQueryRecord rec;
	rec.boundingBox = *getBoundingBox();

	for (auto go : *m_screen->getObjects(GameObjectType::_Enemy)) {
		if (auto other = dynamic_cast<RoyalguardBoss*>(go)) {
			if (other == this) continue;
			rec.boundingBox.left += 20.f;
			other->notifyOtherDeath(getPosition() + (m_level->collides(rec) ? sf::Vector2f() : sf::Vector2f(20.f, 0.f)));
			rec.boundingBox.left -= 20.f;
			break;
		}
	}
}

sf::Time RoyalguardBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

float RoyalguardBoss::getConfiguredDistanceToHPBar() const {
	return 30.f;
}
