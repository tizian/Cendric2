#include "Spells/YashaRaiseTheDeadSpell.h"
#include "Level/Enemies/YashaBossAdd.h"

void YashaRaiseTheDeadSpell::execOnHit(LevelMovableGameObject* target) {
	if (target->getConfiguredType() == _LevelMainCharacter) {
		setDisposed();
		return;
	}

	if (!target->isDead()) return;
	YashaBossAdd* enemy = dynamic_cast<YashaBossAdd*>(target);
	if (enemy == nullptr) return;
	enemy->revive();
	setDisposed();
}

void YashaRaiseTheDeadSpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;

	calculateNextPosition(frameTime, nextPosition);

	checkCollisionsWithAllies(getBoundingBox());
	checkCollisionsWithEnemies(getBoundingBox());

	MovableGameObject::update(frameTime);
	updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}
}

bool YashaRaiseTheDeadSpell::checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) {
	// this method is overridden to guarantee that the spell only hits once
	// and that the iterator is not invalidated (we change the enemy vector size on the fly)
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		YashaBossAdd* enemy = dynamic_cast<YashaBossAdd*>(go);
		if (enemy != nullptr && fastIntersect(*enemy->getBoundingBox(), *boundingBox)) {
			enemy->onHit(this);
			return true;
		}
	}
	return false;
}
