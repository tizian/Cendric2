#include "SpellCreators/ChopSpellCreator.h"
#include "Screens/LevelScreen.h"

ChopSpellCreator::ChopSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
	// no modifiers allowed on this one.
}

void ChopSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);
	ChopSpell* newSpell = new ChopSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	m_owner->setFightAnimationTime();
}