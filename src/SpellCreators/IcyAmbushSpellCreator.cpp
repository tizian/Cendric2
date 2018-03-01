#include "SpellCreators/IcyAmbushSpellCreator.h"
#include "Screens/LevelScreen.h"

IcyAmbushSpellCreator::IcyAmbushSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

std::string IcyAmbushSpellCreator::getStrengthModifierName() const {
	return "StunLevel";
}

void IcyAmbushSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);

	IcyAmbushSpell* newSpell = new IcyAmbushSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}

void IcyAmbushSpellCreator::addRangeModifier(int level) {
	SpellCreator::addRangeModifier(level);
	m_spellData.activeDuration = sf::seconds(m_spellData.range / m_spellData.speed);
}

void IcyAmbushSpellCreator::addSpeedModifier(int level) {
	SpellCreator::addSpeedModifier(level);
	m_spellData.activeDuration = sf::seconds(m_spellData.range / m_spellData.speed);
}

void IcyAmbushSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}