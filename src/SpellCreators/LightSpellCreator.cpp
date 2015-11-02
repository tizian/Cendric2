#include "SpellCreators/LightSpellCreator.h"
#include "Screens/LevelScreen.h"

LightSpellCreator::LightSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner) {
}

void LightSpellCreator::executeSpell(const sf::Vector2f &target) {
	SpellBean spellBean = m_spellBean;
	LightSpell* newSpell = new LightSpell();
	newSpell->load(spellBean, m_owner, target);
	m_screen->addObject(newSpell);

	if (dynamic_cast<LevelMainCharacter*>(m_owner))
		m_screen->addBuffToInterface(BuffType::Spell, spellBean.iconTextureRect, spellBean.duration);
}

void LightSpellCreator::addDurationModifier(int level) {
	m_spellBean.duration += static_cast<float>(level)* m_spellBean.durationModifierAddition;
	m_spellBean.cooldown = m_spellBean.duration;
}