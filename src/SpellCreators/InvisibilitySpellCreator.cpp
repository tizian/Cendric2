#include "SpellCreators/InvisibilitySpellCreator.h"
#include "Screens/LevelScreen.h"

InvisibilitySpellCreator::InvisibilitySpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner) {
	m_invisibilityLevel = 1;
}

int InvisibilitySpellCreator::getStrengthModifierValue() const {
	return m_invisibilityLevel;
}

std::string InvisibilitySpellCreator::getStrengthModifierName() const {
	return "InvisibilityLevel";
}

void InvisibilitySpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellBean spellBean = m_spellBean;
	InvisibilitySpell* newSpell = new InvisibilitySpell();
	newSpell->load(spellBean, m_owner, target);
	m_screen->addObject(newSpell);
	LevelMainCharacter* mainChar = dynamic_cast<LevelMainCharacter*>(m_owner);
	if (mainChar != nullptr) {
		mainChar->setInvisibilityLevel(m_invisibilityLevel);
		m_screen->addBuffToInterface(BuffType::Spell, spellBean.iconTextureRect, spellBean.duration, SpellID::Invisibility);
	}
}

void InvisibilitySpellCreator::addStrengthModifier(int level) {
	m_invisibilityLevel += level;
}