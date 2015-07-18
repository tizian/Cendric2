#include "SpellCreators/AureolaSpellCreator.h"

AureolaSpellCreator::AureolaSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Damage);
	m_allowedModifiers.push_back(SpellModifierType::Count);
	m_allowedModifiers.push_back(SpellModifierType::Speed);
}

void AureolaSpellCreator::executeSpell(sf::Vector2f target)
{
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);
	for (int i = 0; i < m_spellBean.count; i++)
	{
		AureolaSpell* newSpell = new AureolaSpell();
		newSpell->load(spellBean, m_owner, g_inputController->getMousePosition(), i  * spellBean.divergenceAngle);
		m_screen->addObject(GameObjectType::_Spell, newSpell);
	}

	m_owner->setFightAnimationTime();
}

void AureolaSpellCreator::addCountModifier(int level)
{
	m_spellBean.count += m_spellBean.countModifierAddition * level;
	m_spellBean.divergenceAngle = 2 * PI_F / m_spellBean.count;
}