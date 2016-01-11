#include "SpellCreators/ShadowTrapSpellCreator.h"
#include "Screens/LevelScreen.h"

ShadowTrapSpellCreator::ShadowTrapSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

int ShadowTrapSpellCreator::getStrengthModifierValue() const {
	return m_strength;
}

std::string ShadowTrapSpellCreator::getStrengthModifierName() const {
	return "Strength";
}

void ShadowTrapSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);

	ShadowTrapSpell* newSpell = new ShadowTrapSpell(m_strength);

	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}

void ShadowTrapSpellCreator::addStrengthModifier(int level) {
	m_strength += level;
}

void ShadowTrapSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}

void ShadowTrapSpellCreator::addDamageModifier(int level) {
	m_spellData.damagePerSecond += m_spellData.damageModifierAddition * level;
}