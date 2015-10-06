#include "SpellCreators/UnlockSpellCreator.h"
#include "Screens/LevelScreen.h"

UnlockSpellCreator::UnlockSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner) {
}

int UnlockSpellCreator::getStrengthModifierValue() const {
	return m_strength;
}

std::string UnlockSpellCreator::getStrengthModifierName() const {
	return "LockpickLevel";
}

void UnlockSpellCreator::executeSpell(const sf::Vector2f& target) {
	SpellBean spellBean = m_spellBean;

	UnlockSpell* newSpell = new UnlockSpell(m_strength);
	newSpell->load(spellBean, m_owner, target);
	m_screen->addObject(newSpell);

	m_owner->setFightAnimationTime();
}

void UnlockSpellCreator::addStrengthModifier(int level) {
	m_strength += level;
}