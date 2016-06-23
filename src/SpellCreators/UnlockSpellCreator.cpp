#include "SpellCreators/UnlockSpellCreator.h"
#include "Screens/LevelScreen.h"

UnlockSpellCreator::UnlockSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

int UnlockSpellCreator::getStrengthModifierValue() const {
	return m_spellData.strength;
}

std::string UnlockSpellCreator::getStrengthModifierName() const {
	return "LockpickLevel";
}

void UnlockSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;

	UnlockSpell* newSpell = new UnlockSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}

void UnlockSpellCreator::addStrengthModifier(int level) {
	m_spellData.strength += level;
}