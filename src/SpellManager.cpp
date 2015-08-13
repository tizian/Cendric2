#include "SpellManager.h"
#include "GUI/SpellSelection.h"

using namespace std;

SpellManager::SpellManager(LevelMovableGameObject* owner)
{
	m_currentSpell = -1;
	m_owner = owner;
}

SpellManager::~SpellManager()
{
	clearSpells();
}

void SpellManager::clearSpells()
{
	for (auto& it : m_spellMap)
	{
		delete it;
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
	m_spellMap.push_back(getSpellCreator(spell, modifiers));
	m_coolDownMap.push_back(sf::Time::Zero);
}

void SpellManager::executeCurrentSpell(const sf::Vector2f& target)
{
	if (m_currentSpell == -1 || m_coolDownMap[m_currentSpell].asMilliseconds() != 0) return;
	
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
	for (auto &it : m_coolDownMap) {
		if (it.asMilliseconds() == 0) continue;
		it -= frameTime;
		if (it.asMilliseconds() < 0) it = sf::Time::Zero;
	}
}

void SpellManager::setCurrentSpell(int spellNr)
{
	if (spellNr < -1 || spellNr > m_spellMap.size() - 1)
	{
		g_logger->logError("SpellManager::setCurrentSpell", "A invalid spell is set as current spell. Spell nr: " + to_string(spellNr));
		m_currentSpell = -1;
		return;
	}
	m_currentSpell = spellNr;
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
	case SpellID::Fear:
		creator = new FearSpellCreator(bean, m_owner);
		break;
	case SpellID::AntiGravity:
		creator = new AntiGravitySpellCreator(bean, m_owner);
		break;
	default:
		return nullptr;
	}

	creator->addModifiers(modifiers);
	
	return creator;
}

std::vector<SpellCreator*>& SpellManager::getSpellMap()
{
	return m_spellMap;
}