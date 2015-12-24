#include "SpellCreators/DivineShieldSpellCreator.h"
#include "Screens/LevelScreen.h"

DivineShieldSpellCreator::DivineShieldSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
	m_additionalResistance = 10;
}

int DivineShieldSpellCreator::getStrengthModifierValue() const {
	return m_additionalResistance;
}

std::string DivineShieldSpellCreator::getStrengthModifierName() const {
	return "AdditionalResistance";
}

void DivineShieldSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData= m_spellData;
	DivineShieldSpell* newSpell = new DivineShieldSpell(m_additionalResistance);
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	if (dynamic_cast<LevelMainCharacter*>(m_owner))
		m_screen->addBuffToInterface(BuffType::Spell, spellData.iconTextureRect, spellData.duration);

	m_owner->addHeal(newSpell->getHeal());
}

void DivineShieldSpellCreator::addStrengthModifier(int level) {
	m_spellData.heal += 10 * level;
	m_additionalResistance += 10 * level;
}