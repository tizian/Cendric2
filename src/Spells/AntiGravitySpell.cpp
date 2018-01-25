#include "Spells/AntiGravitySpell.h"
#include "Screens/LevelScreen.h"

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
}

void AntiGravitySpell::setDisposed() {
	m_mob->flipGravity();
	Spell::setDisposed();
	if (m_mob->getConfiguredType() == _LevelMainCharacter) {
		dynamic_cast<LevelScreen*>(m_screen)->removeTypedBuffs(SpellID::AntiGravity);
	}
}

bool AntiGravitySpell::getConfiguredRotateSprite() const {
	return false;
}