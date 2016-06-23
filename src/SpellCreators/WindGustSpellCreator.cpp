#include "SpellCreators/WindGustSpellCreator.h"
#include "Screens/LevelScreen.h"

WindGustSpellCreator::WindGustSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

int WindGustSpellCreator::getStrengthModifierValue() const {
	return m_spellData.strength;
}

std::string WindGustSpellCreator::getStrengthModifierName() const {
	return "WindForce";
}

void WindGustSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);
	WindGustSpell* newSpell = new WindGustSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}

void WindGustSpellCreator::addRangeModifier(int level) {
	m_spellData.boundingBox.width += m_spellData.rangeModifierAddition * level;
}

void WindGustSpellCreator::addStrengthModifier(int level) {
	m_spellData.ccStrength += level;
	m_spellData.strength += level;
}

void WindGustSpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level) * m_spellData.durationModifierAddition;
	m_spellData.duration = m_spellData.activeDuration;
}
