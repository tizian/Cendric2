#include "SpellCreators/ChopSpellCreator.h"

ChopSpellCreator::ChopSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner)
{
	// no modifiers allowed on this one.
}

void ChopSpellCreator::executeSpell(const sf::Vector2f &target)
{
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);
	ChopSpell* newSpell = new ChopSpell();
	newSpell->load(spellBean, m_owner, target, 0);
	m_screen->addObject(GameObjectType::_Spell, newSpell);
	m_owner->setFightAnimationTime();
}