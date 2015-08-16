#include "SpellCreators/DivineShieldSpellCreator.h"
#include "Screens/LevelScreen.h"

DivineShieldSpellCreator::DivineShieldSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Duration);
	m_allowedModifiers.push_back(SpellModifierType::Strength);
	m_additionalResistance = 10;
}

void DivineShieldSpellCreator::executeSpell(const sf::Vector2f &target)
{
	SpellBean spellBean = m_spellBean;
	DivineShieldSpell* newSpell = new DivineShieldSpell(m_additionalResistance);
	newSpell->load(spellBean, m_owner, target, 0);
	m_screen->addObject(newSpell);
	m_screen->addBuffToInterface(BuffType::Spell, spellBean.iconTextureRect, spellBean.duration);
	
	m_owner->addHeal(newSpell->getHeal());
}

void DivineShieldSpellCreator::addStrengthModifier(int level)
{
	m_spellBean.heal += 10 * level;
	m_additionalResistance += 10 * level;
}