#include "Spells/AntiGravitySpell.h"

AntiGravitySpell::AntiGravitySpell() : Spell() {
}

void AntiGravitySpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_mob->flipGravity();
}

void AntiGravitySpell::render(sf::RenderTarget& target) {
	// nop
}

void AntiGravitySpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition, m_mob);
	setPosition(nextPosition);
	if (m_isDebugRendering) {
		m_debugBox.setPosition(getPosition());
	}

	updateTime(m_data.activeDuration, frameTime);

	if (m_data.activeDuration.asMilliseconds() <= 0) {
		setDisposed();
	}
}

void AntiGravitySpell::setDisposed() {
	m_mob->flipGravity();
	Spell::setDisposed();
}

bool AntiGravitySpell::getConfiguredRotateSprite() const {
	return false;
}