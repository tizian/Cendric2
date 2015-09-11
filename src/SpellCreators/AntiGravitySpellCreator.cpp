#include "SpellCreators/AntiGravitySpellCreator.h"
#include "Screens/LevelScreen.h"

AntiGravitySpellCreator::AntiGravitySpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner)
{
}

void AntiGravitySpellCreator::executeSpell(const sf::Vector2f& target)
{
	SpellBean spellBean = m_spellBean;
	AntiGravitySpell* newSpell = new AntiGravitySpell();
	newSpell->load(spellBean, m_owner, target);
	m_screen->addObject(newSpell);
	if (dynamic_cast<LevelMainCharacter*>(m_owner))
		m_screen->addBuffToInterface(BuffType::Spell, spellBean.iconTextureRect, spellBean.duration);
}