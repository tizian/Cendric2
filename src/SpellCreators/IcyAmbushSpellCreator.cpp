#include "SpellCreators/IcyAmbushSpellCreator.h"
#include "Screens/LevelScreen.h"

IcyAmbushSpellCreator::IcyAmbushSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner)
{
}

void IcyAmbushSpellCreator::executeSpell(const sf::Vector2f& target)
{
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);

	IcyAmbushSpell* newSpell = new IcyAmbushSpell(m_stunTime);
	newSpell->load(spellBean, m_owner, target);
	m_screen->addObject(newSpell);
		
	m_owner->setFightAnimationTime();
}

void IcyAmbushSpellCreator::addDurationModifier(int level)
{
	m_stunTime += static_cast<float>(level)* m_spellBean.durationModifierAddition;
}