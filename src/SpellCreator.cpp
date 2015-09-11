#include "SpellCreator.h"
#include "Screens/LevelScreen.h"

SpellCreator::SpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner)
{
	m_owner = owner;
	m_level = owner->getLevel();
	m_attributeBean = owner->getAttributes();
	if (!(m_screen = dynamic_cast<LevelScreen*>(owner->getScreen())))
	{
		g_logger->logError("SpellCreator", "spell owner has no (level)screen");
	}
	m_spellBean = spellBean;

	m_allowedModifiers = SpellBean::getAllowedModifiers(spellBean.id);
}

SpellCreator::~SpellCreator()
{
}

void SpellCreator::addModifiers(const std::vector<SpellModifier>& modifiers)
{
	for (auto& it : modifiers)
	{
		if (it.type == SpellModifierType::VOID) continue;
		// some security checks
		if (!(std::find(m_allowedModifiers.begin(), m_allowedModifiers.end(), it.type) != m_allowedModifiers.end()))
		{
			g_logger->logWarning("SpellCreator", "Modifier of an unallowed type was ignored.");
			continue;
		}
		if (it.level < 1 || it.level > 3)
		{
			g_logger->logWarning("SpellCreator", "Modifier of an unallowed level was ignored.");
			continue;
		}
		switch (it.type)
		{
		case SpellModifierType::Count:
			addCountModifier(it.level);
			break;
		case SpellModifierType::Damage:
			addDamageModifier(it.level);
			break;
		case SpellModifierType::Range:
			addRangeModifier(it.level);
			break;
		case SpellModifierType::Reflect:
			addReflectModifier(it.level);
			break;
		case SpellModifierType::Speed:
			addSpeedModifier(it.level);
			break;
		case SpellModifierType::Strength:
			addStrengthModifier(it.level);
			break;
		case SpellModifierType::Duration:
			addDurationModifier(it.level);
			break;
		default:
			g_logger->logWarning("SpellCreator", "Modifier of an unknown type was ignored.");
			break;
		}
	}
}

void SpellCreator::addSpeedModifier(int level)
{
	m_spellBean.startVelocity += m_spellBean.speedModifierAddition * level;
}

void SpellCreator::addDamageModifier(int level)
{
	m_spellBean.damage += m_spellBean.damageModifierAddition * level;
}

void SpellCreator::addDurationModifier(int level)
{
	m_spellBean.duration += static_cast<float>(level) * m_spellBean.durationModifierAddition;
}

void SpellCreator::addRangeModifier(int level)
{
	m_spellBean.range += m_spellBean.rangeModifierAddition * level;
}

void SpellCreator::addCountModifier(int level)
{
	m_spellBean.count += m_spellBean.countModifierAddition * level;
}

void SpellCreator::addReflectModifier(int level)
{
	m_spellBean.reflectCount += m_spellBean.reflectModifierAddition * level;
}

void SpellCreator::addStrengthModifier(int level)
{
	// nop, we don't know yet what this does but the method is implemented so its subclasses won't have to
}

const SpellBean& SpellCreator::getSpellBean() const
{
	return m_spellBean;
}

void SpellCreator::updateDamage(SpellBean& bean) const
{
	switch (bean.damageType)
	{
	case DamageType::Physical:
		bean.damage = bean.damage + m_attributeBean->damagePhysical;
		break;
	case DamageType::Fire:
		bean.damage = bean.damage + m_attributeBean->damageFire;
		break;
	case DamageType::Ice:
		bean.damage = bean.damage + m_attributeBean->damageIce;
		break;
	case DamageType::Shadow:
		bean.damage = bean.damage + m_attributeBean->damageShadow;
		break;
	case DamageType::Light:
		bean.damage = bean.damage + m_attributeBean->damageLight;
		break;
	default:
		return;
	}

	// add randomness to damage (something from 80 - 120% of the base damage)
	bean.damage = static_cast<int>(bean.damage * ((rand() % 41 + 80.f) / 100.f));

	// add critical hit to damage
	int chance = rand() % 100 + 1;
	if (chance <= m_attributeBean->criticalHitChance)
	{
		bean.damage *= 2;
	}
}