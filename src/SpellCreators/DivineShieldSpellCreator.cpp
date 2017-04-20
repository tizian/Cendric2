#include "SpellCreators/DivineShieldSpellCreator.h"
#include "Screens/LevelScreen.h"

DivineShieldSpellCreator::DivineShieldSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
	m_additionalResistance = 10;
}

int DivineShieldSpellCreator::getStrengthModifierValue() const {
	return m_additionalResistance;
}

std::string DivineShieldSpellCreator::getStrengthModifierName() const {
	return "AdditionalResistance";
}

void DivineShieldSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);
	AttributeData resistance;
	resistance.resistancePhysical = m_additionalResistance;
	resistance.resistanceFire = m_additionalResistance;
	resistance.resistanceIce = m_additionalResistance;
	resistance.resistanceShadow = m_additionalResistance;
	resistance.resistanceLight = m_additionalResistance;
	DivineShieldSpell* newSpell = new DivineShieldSpell(resistance);
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	if (dynamic_cast<LevelMainCharacter*>(m_owner)) {
		m_screen->addSpellBuffToInterface(spellData.iconTextureRect, spellData.duration, newSpell, resistance);
	}
}

void DivineShieldSpellCreator::addStrengthModifier(int level) {
	m_spellData.heal += 10 * level;
	m_additionalResistance += 10 * level;
}

void DivineShieldSpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level) * m_spellData.durationModifierAddition;
	m_spellData.duration = m_spellData.activeDuration;
}