#include "SpellCreators/AntiGravitySpellCreator.h"
#include "Screens/LevelScreen.h"

AntiGravitySpellCreator::AntiGravitySpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Duration);
}

void AntiGravitySpellCreator::executeSpell(const sf::Vector2f &target)
{
	SpellBean spellBean = m_spellBean;
	AntiGravitySpell* newSpell = new AntiGravitySpell();
	newSpell->load(spellBean, m_owner, target, 0);
	m_screen->addObject(newSpell);
	m_screen->addBuffToInterface(BuffType::Spell, spellBean.iconTextureRect, spellBean.duration);
}