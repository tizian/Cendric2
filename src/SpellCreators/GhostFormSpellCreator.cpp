#include "SpellCreators/GhostFormSpellCreator.h"
#include "Screens/LevelScreen.h"

GhostFormSpellCreator::GhostFormSpellCreator(const SpellData& spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
	m_additionalChopDamage = 10;
}

int GhostFormSpellCreator::getStrengthModifierValue() const {
	return m_additionalChopDamage;
}

std::string GhostFormSpellCreator::getStrengthModifierName() const {
	return "GhostDamage";
}

void GhostFormSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	AttributeData additionalDamage = ZERO_ATTRIBUTES;
	additionalDamage.damagePhysical = m_additionalChopDamage;
	GhostFormSpell* newSpell = new GhostFormSpell(additionalDamage);
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	LevelMainCharacter* mainChar = dynamic_cast<LevelMainCharacter*>(m_owner);
	if (mainChar != nullptr) {
		m_screen->addSpellBuffToInterface(spellData.iconTextureRect, spellData.duration, newSpell, additionalDamage);
	}
}

void GhostFormSpellCreator::addStrengthModifier(int level) {
	m_additionalChopDamage += level * 10;
}

void GhostFormSpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level) * m_spellData.durationModifierAddition;
	m_spellData.duration = m_spellData.activeDuration;
}