#include "SpellManager.h"

using namespace std;

SpellManager::SpellManager(const AttributeBean* bean)
{
	m_currentSpell = SpellID::Chop;
	m_attributeBean = bean;
}

SpellManager::~SpellManager()
{
	m_spellMap.clear();
	m_coolDownMap.clear();
}

void SpellManager::addSpell(SpellBean& spell)
{
	m_spellMap.insert({ spell.id, spell });
	m_coolDownMap.insert({ spell.id, sf::Time::Zero });
}

const std::vector<Spell*>& SpellManager::getSpells()
{
	m_spellHolder.clear();

	if (m_coolDownMap[m_currentSpell].asMilliseconds() != 0) return m_spellHolder;
	
	// spell has been cast. set cooldown.
	m_coolDownMap[m_currentSpell] = m_spellMap[m_currentSpell].cooldown * m_attributeBean->cooldownMultiplier;

	for (int i = 0; i < m_spellMap[m_currentSpell].amount; i++)
	{
		Spell* newSpell;
		switch (m_currentSpell)
		{
		case SpellID::Chop:
			newSpell = new ChopSpell();
			break;
		case SpellID::Fire:
			newSpell = new FireSpell();
			break;
		case SpellID::Ice:
			newSpell = new IceSpell();
			break;
		case SpellID::Forcefield:
			newSpell = new ForcefieldSpell();
			break;
		case SpellID::Unlock:
			newSpell = new UnlockSpell();
			break;
		default:
			return m_spellHolder;
		}

		SpellBean spellBean = m_spellMap[m_currentSpell];
		updateDamage(newSpell->getConfiguredDamageType(), spellBean);
		newSpell->init(spellBean);
		m_spellHolder.push_back(newSpell);
	}

	return m_spellHolder;
}

void SpellManager::update(sf::Time frameTime)
{
	// update cooldown map
	for (auto const &it : m_coolDownMap) {
		if (m_coolDownMap[it.first].asMilliseconds() == 0) continue;
		m_coolDownMap[it.first] = it.second - frameTime;
		if (m_coolDownMap[it.first].asMilliseconds() < 0) m_coolDownMap[it.first] = sf::Time::Zero;
	}
}

void SpellManager::setCurrentSpell(SpellID id)
{
	m_currentSpell = id;
}

void SpellManager::updateDamage(DamageType type, SpellBean& bean) const
{
	switch (type)
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

void SpellManager::render(sf::RenderTarget &renderTarget)
{
	// TODO render gui
}
