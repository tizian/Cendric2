#include "SpellManager.h"

using namespace std;

SpellManager::SpellManager()
{
	m_currentSpell = SpellID::Chop;
	init();
}

SpellManager::~SpellManager()
{
	m_spellMap.clear();
	m_coolDownMap.clear();
	m_keyMap.clear();
}

void SpellManager::init()
{
	// TODO these values should come from a staff. they are hardcoded for now.
	SpellBean fireSpell;
	fireSpell.maxActiveTime = sf::milliseconds(5000);
	fireSpell.cooldown = sf::milliseconds(1000);
	fireSpell.damage = 10;
	fireSpell.reflectCount = 0;
	fireSpell.startVelocity = 500.f;

	SpellBean chopSpell;
	chopSpell.maxActiveTime = sf::milliseconds(320);
	chopSpell.cooldown = sf::milliseconds(400);
	chopSpell.damage = 2;
	chopSpell.reflectCount = 0;
	chopSpell.startVelocity = 0.f;
	chopSpell.boundingBox = sf::FloatRect(0, 0, 40, 60);

	SpellBean forcefieldSpell;
	forcefieldSpell.maxActiveTime = sf::milliseconds(3000);
	forcefieldSpell.cooldown = sf::milliseconds(10000);
	forcefieldSpell.damage = 1;
	forcefieldSpell.reflectCount = 0;
	forcefieldSpell.startVelocity = 0.f;

	SpellBean iceSpell;
	iceSpell.maxActiveTime = sf::milliseconds(5000);
	iceSpell.cooldown = sf::milliseconds(1000);
	iceSpell.damage = 6;
	iceSpell.reflectCount = 0;
	iceSpell.startVelocity = 700.f;

	m_spellMap.insert(
	{
		{ SpellID::Chop, chopSpell },
		{ SpellID::Fire, fireSpell },
		{ SpellID::Ice, iceSpell },
		{ SpellID::Forcefield, forcefieldSpell }
	});
	m_coolDownMap.insert(
	{
		{ SpellID::Chop, sf::Time::Zero },
		{ SpellID::Fire, sf::Time::Zero },
		{ SpellID::Ice, sf::Time::Zero },
		{ SpellID::Forcefield, sf::Time::Zero }
	});
	m_keyMap.insert(
	{
		{ Key::SpellChop, SpellID::Chop },
		{ Key::SpellIce, SpellID::Ice },
		{ Key::SpellFire, SpellID::Fire },
		{ Key::SpellForcefield, SpellID::Forcefield }
	});

}

void SpellManager::update(sf::Time frameTime)
{
	// update current spell
	for (auto const &it : m_keyMap) {
		if (g_inputController->isKeyActive(it.first))
		{
			m_currentSpell = it.second;
		}
	}

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

void SpellManager::render(sf::RenderTarget &renderTarget)
{
	// TODO render gui
}
