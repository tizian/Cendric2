#include "Spells/BuffSpell.h"

BuffSpell::BuffSpell(const AttributeData& buff) : Spell() {
	m_buff = buff;
}

void BuffSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_mob->addAttributes(m_data.duration, m_buff);
	m_mob->addHeal(getHeal(), false);
}

void BuffSpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition);
	setPosition(nextPosition);

	MovableGameObject::update(frameTime);
	updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration.asMilliseconds() <= 0) {
		setDisposed();
	}
}

bool BuffSpell::getConfiguredRotateSprite() const {
	return false;
}

void BuffSpell::execOnHit(LevelMovableGameObject* target) {
	// this spell buffs other enemies!
	if (target == m_mob) return;
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		Enemy* owner = dynamic_cast<Enemy*>(m_mob);
		if (enemy->getEnemyID() == owner->getEnemyID()) {
			enemy->addAttributes(m_data.duration, m_buff);
			enemy->addHeal(getHeal(), false);
			setDisposed();
		}
	}
}