#include "SpellCreators/FearSpellCreator.h"

FearSpellCreator::FearSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Count);
	m_allowedModifiers.push_back(SpellModifierType::Reflect);
	m_allowedModifiers.push_back(SpellModifierType::Speed);
	m_allowedModifiers.push_back(SpellModifierType::Duration);
	m_allowedModifiers.push_back(SpellModifierType::Strength);
}

void FearSpellCreator::executeSpell(const sf::Vector2f &target)
{
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);
	int div = 0;
	int sign = 1;
	for (int i = 0; i < m_spellBean.count; i++)
	{
		FearSpell* newSpell = new FearSpell(m_fearedDuration, m_strength);
		newSpell->load(spellBean, m_owner, target, div * sign * spellBean.divergenceAngle);
		m_screen->addObject(GameObjectType::_Spell, newSpell);
		sign = -sign;
		if (sign == -1)
		{
			div += 1;
		}
	}

	m_owner->setFightAnimationTime();
}

void FearSpellCreator::addDurationModifier(int level)
{
	m_fearedDuration += static_cast<float>(level)* m_spellBean.durationModifierAddition;
}

void FearSpellCreator::addStrengthModifier(int level)
{
	m_strength += level;
}