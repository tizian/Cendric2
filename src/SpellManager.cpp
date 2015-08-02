#include "SpellManager.h"
#include "GUI/SpellSelection.h"

using namespace std;

SpellManager::SpellManager(LevelMovableGameObject* owner)
{
	m_currentSpell = SpellID::Chop;
	m_owner = owner;
}

SpellManager::~SpellManager()
{
	for (auto& it : m_spellMap)
	{
		delete it.second;
	}
	m_spellMap.clear();
	m_coolDownMap.clear();
}

void SpellManager::setSpellSelection(SpellSelection* selection)
{
	m_spellSelection = selection;
}

void SpellManager::addSpell(const SpellBean& spell)
{
	addSpell(spell, std::vector<SpellModifier>());
}

void SpellManager::addSpell(const SpellBean& spell, const std::vector<SpellModifier>& modifiers)
{
	// only one spell is allowed per spell id. This check is only
	// to assure that there are no more spellcreators created with "new" than
	// there are deleted in the destructor.
	if (m_spellMap.find(spell.id) == m_spellMap.end())
	{
		m_spellMap.insert({ spell.id, getSpellCreator(spell, modifiers) });
		m_coolDownMap.insert({ spell.id, sf::Time::Zero });
	}
}

void SpellManager::executeCurrentSpell(const sf::Vector2f& target)
{
	if (m_currentSpell == SpellID::VOID || m_coolDownMap[m_currentSpell].asMilliseconds() != 0) return;
	
	// spell has been cast. set cooldown.
	sf::Time cooldown = m_spellMap[m_currentSpell]->getSpellBean().cooldown * m_owner->getAttributes()->cooldownMultiplier;
	m_coolDownMap[m_currentSpell] = cooldown;
	m_spellMap[m_currentSpell]->executeSpell(target);
	if (m_spellSelection != nullptr)
	{
		m_spellSelection->activateSlot(m_currentSpell, cooldown);
	}
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

SpellCreator* SpellManager::getSpellCreator(const SpellBean& bean, const std::vector<SpellModifier>& modifiers)
{
	SpellCreator* creator;
	switch (bean.id)
	{
	case SpellID::Chop:
		creator = new ChopSpellCreator(bean, m_owner);
		break;
	case SpellID::FireBall:
		creator = new FireBallSpellCreator(bean, m_owner);
		break;
	case SpellID::IceBall:
		creator = new IceBallSpellCreator(bean, m_owner);
		break;
	case SpellID::DivineShield:
		creator = new DivineShieldSpellCreator(bean, m_owner);
		break;
	case SpellID::Aureola:
		creator = new AureolaSpellCreator(bean, m_owner);
		break;
	default:
		return nullptr;
	}

	creator->addModifiers(modifiers);
	
	return creator;
}

std::map<SpellID, SpellCreator*>& SpellManager::getSpellMap()
{
	return m_spellMap;
}