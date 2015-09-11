#include "SpellCreators/FearSpellCreator.h"
#include "Screens/LevelScreen.h"

FearSpellCreator::FearSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner)
{
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
		spellBean.divergenceAngle = div * sign * m_spellBean.divergenceAngle;
		newSpell->load(spellBean, m_owner, target);
		m_screen->addObject(newSpell);
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