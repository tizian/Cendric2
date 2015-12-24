#include "SpellCreators/InvisibilitySpellCreator.h"
#include "Screens/LevelScreen.h"

InvisibilitySpellCreator::InvisibilitySpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
	m_invisibilityLevel = 1;
}

int InvisibilitySpellCreator::getStrengthModifierValue() const {
	return m_invisibilityLevel;
}

std::string InvisibilitySpellCreator::getStrengthModifierName() const {
	return "InvisibilityLevel";
}

void InvisibilitySpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	InvisibilitySpell* newSpell = new InvisibilitySpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
	LevelMainCharacter* mainChar = dynamic_cast<LevelMainCharacter*>(m_owner);
	if (mainChar != nullptr) {
		mainChar->setInvisibilityLevel(m_invisibilityLevel);
		m_screen->addBuffToInterface(BuffType::Spell, spellData.iconTextureRect, spellData.duration, SpellID::Invisibility);
	}
}

void InvisibilitySpellCreator::addStrengthModifier(int level) {
	m_invisibilityLevel += level;
}