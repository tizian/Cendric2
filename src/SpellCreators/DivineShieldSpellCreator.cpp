#include "SpellCreators/DivineShieldSpellCreator.h"

DivineShieldSpellCreator::DivineShieldSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Duration);
	m_allowedModifiers.push_back(SpellModifierType::Strength);
	m_additionalResistance = 10;
}

void DivineShieldSpellCreator::executeSpell(sf::Vector2f target)
{
	SpellBean spellBean = m_spellBean;
	DivineShieldSpell* newSpell = new DivineShieldSpell(m_additionalResistance);
	newSpell->load(spellBean, m_owner, g_inputController->getMousePosition(), 0);
	m_screen->addObject(GameObjectType::_Spell, newSpell);
	
	m_owner->addHeal(newSpell->getHeal());
}

void DivineShieldSpellCreator::addStrengthModifier(int level)
{
	m_spellBean.heal += 10 * level;
	m_additionalResistance += 10 * level;
}