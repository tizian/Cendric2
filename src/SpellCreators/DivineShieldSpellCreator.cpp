#include "SpellCreators/DivineShieldSpellCreator.h"

DivineShieldSpellCreator::DivineShieldSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Duration);
	m_allowedModifiers.push_back(SpellModifierType::Strength);
}

void DivineShieldSpellCreator::executeSpell(sf::Vector2f target)
{
	SpellBean spellBean = m_spellBean;
	DivineShieldSpell* newSpell = new DivineShieldSpell();
	newSpell->load(spellBean, m_owner, g_inputController->getMousePosition(), 0);
	m_screen->addObject(GameObjectType::_Spell, newSpell);
	// TODO heal owner
}

// TODO override modifier strength