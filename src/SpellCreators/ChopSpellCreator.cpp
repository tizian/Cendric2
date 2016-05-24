#include "SpellCreators/ChopSpellCreator.h"
#include "Screens/LevelScreen.h"

ChopSpellCreator::ChopSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
	// no modifiers allowed on this one.
}

void ChopSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);
	ChopSpell* newSpell = new ChopSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}