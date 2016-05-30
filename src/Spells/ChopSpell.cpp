#include "Spells/ChopSpell.h"

ChopSpell::ChopSpell() : Spell() {
}

void ChopSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	Spell::load(bean, mob, target);
}

bool ChopSpell::getConfiguredRotateSprite() const {
	return false;
}