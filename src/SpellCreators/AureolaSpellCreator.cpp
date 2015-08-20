#include "SpellCreators/AureolaSpellCreator.h"
#include "Screens/LevelScreen.h"

AureolaSpellCreator::AureolaSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Damage);
	m_allowedModifiers.push_back(SpellModifierType::Count);
	m_allowedModifiers.push_back(SpellModifierType::Speed);
}

void AureolaSpellCreator::executeSpell(const sf::Vector2f &target)
{
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);
	for (int i = 0; i < m_spellBean.count; i++)
	{
		AureolaSpell* newSpell = new AureolaSpell();
		spellBean.divergenceAngle = i * m_spellBean.divergenceAngle;
		newSpell->load(spellBean, m_owner, target);
		m_screen->addObject(newSpell);
	}

	m_owner->setFightAnimationTime();
}

void AureolaSpellCreator::addCountModifier(int level)
{
	m_spellBean.count += m_spellBean.countModifierAddition * level;
	m_spellBean.divergenceAngle = 2 * PI_F / m_spellBean.count;
}