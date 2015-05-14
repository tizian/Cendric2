#include "SpellManager.h"

using namespace std;

SpellManager::SpellManager()
{
	m_currentSpell = SpellID::Chop;
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

void SpellManager::update(sf::Time frameTime)
{
	// update cooldown map
	for (auto const &it : m_coolDownMap) {
		if (m_coolDownMap[it.first].asMilliseconds() == 0) continue;
		m_coolDownMap[it.first] = it.second - frameTime;
		if (m_coolDownMap[it.first].asMilliseconds() < 0) m_coolDownMap[it.first] = sf::Time::Zero;
	}
}

Spell* SpellManager::getSpell()
{
	if (m_coolDownMap[m_currentSpell].asMilliseconds() != 0) return nullptr;
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
	default:
		return nullptr;
	}

	newSpell->init(m_spellMap[m_currentSpell]);
	// spell has been cast. set cooldown.
	m_coolDownMap[m_currentSpell] = m_spellMap[m_currentSpell].cooldown;
	return newSpell;
}

void SpellManager::setCurrentSpell(SpellID id)
{
	m_currentSpell = id;
}

void SpellManager::render(sf::RenderTarget &renderTarget)
{
	// TODO render gui
}
