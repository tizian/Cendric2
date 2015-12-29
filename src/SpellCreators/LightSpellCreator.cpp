#include "SpellCreators/LightSpellCreator.h"
#include "Screens/LevelScreen.h"

LightSpellCreator::LightSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner) : SpellCreator(spellData, owner) {
}

void LightSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellData spellData = m_spellData;
	LightSpell* newSpell = new LightSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);

	if (dynamic_cast<LevelMainCharacter*>(m_owner))
		m_screen->addBuffToInterface(BuffType::Spell, spellData.iconTextureRect, spellData.duration);
}

void LightSpellCreator::addDurationModifier(int level) {
	m_spellData.activeDuration += static_cast<float>(level)* m_spellData.durationModifierAddition;
	m_spellData.duration = m_spellData.activeDuration;
	m_spellData.cooldown = m_spellData.activeDuration;
}