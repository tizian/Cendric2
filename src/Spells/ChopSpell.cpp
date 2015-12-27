#include "Spells/ChopSpell.h"

ChopSpell::ChopSpell() : Spell() {
}

void ChopSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	m_spellOffset.x = bean.boundingBox.left;
	m_spellOffset.y = bean.boundingBox.top;
	Spell::load(bean, mob, target);
}

const sf::Vector2f ChopSpell::getConfiguredPositionOffset() const {
	return m_spellOffset;
}

bool ChopSpell::getConfiguredRotateSprite() const {
	return false;
}