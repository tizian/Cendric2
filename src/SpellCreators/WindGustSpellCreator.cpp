#include "SpellCreators/WindGustSpellCreator.h"
#include "Screens/LevelScreen.h"

WindGustSpellCreator::WindGustSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

int WindGustSpellCreator::getStrengthModifierValue() const {
	return m_strength;
}

std::string WindGustSpellCreator::getStrengthModifierName() const {
	return "WindForce";
}

void WindGustSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	updateDamage(spellData);
	WindGustSpell* newSpell = new WindGustSpell(m_strength);
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	m_owner->setFightAnimationTime();
}

void WindGustSpellCreator::addRangeModifier(int level) {
	m_spellData.boundingBox.width += m_spellData.rangeModifierAddition * level;
}

void WindGustSpellCreator::addStrengthModifier(int level) {
	m_strength += level;
}

void WindGustSpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level) * m_spellData.durationModifierAddition;
	m_spellData.duration = m_spellData.activeDuration;
}

void WindGustSpellCreator::addDamageModifier(int level) {
	m_spellData.damagePerSecond += static_cast<int>(std::floor(m_spellData.damageModifierAddition / m_spellData.duration.asSeconds())) * level;
}