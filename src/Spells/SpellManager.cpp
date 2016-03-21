#include "Spells/SpellManager.h"
#include "GUI/SpellSelection.h"

using namespace std;

SpellManager::SpellManager(LevelMovableGameObject* owner) {
	m_currentSpell = -1;
	m_owner = owner;
}

SpellManager::~SpellManager() {
	clearSpells();
}

void SpellManager::clearSpells() {
	for (auto& it : m_spellMap) {
		delete it;
	}
	m_spellMap.clear();
	m_coolDownMap.clear();
}

void SpellManager::setSpellSelection(SpellSelection* selection) {
	m_spellSelection = selection;
}

void SpellManager::setSpellsAllied(bool value) {
	for (auto& spellcreator : m_spellMap) {
		spellcreator->setSpellAllied(value);
	}
}

void SpellManager::addSpell(const SpellData& spell) {
	addSpell(spell, std::vector<SpellModifier>());
}

void SpellManager::addSpell(const SpellData& spell, const std::vector<SpellModifier>& modifiers) {
	m_spellMap.push_back(SpellData::getSpellCreator(spell, modifiers, m_owner));
	m_coolDownMap.push_back(sf::Time::Zero);
}

void SpellManager::executeCurrentSpell(const sf::Vector2f& target) {
	if (m_currentSpell == -1 || m_coolDownMap[m_currentSpell].asMilliseconds() != 0) return;

	// spell has been cast. set cooldown.
	sf::Time cooldown = m_spellMap[m_currentSpell]->getSpellData().cooldown * m_owner->getAttributes()->cooldownMultiplier;
	m_coolDownMap[m_currentSpell] = cooldown;
	m_spellMap[m_currentSpell]->executeSpell(target);
	if (m_spellSelection != nullptr) {
		m_spellSelection->activateSlot(m_currentSpell, cooldown);
	}
}

void SpellManager::update(sf::Time frameTime) {
	// update cooldown map
	for (auto& it : m_coolDownMap) {
		if (it.asMilliseconds() == 0) continue;
		it -= frameTime;
		if (it.asMilliseconds() < 0) it = sf::Time::Zero;
	}
}

void SpellManager::setAndExecuteSpell(int spellNr) {
	setCurrentSpell(spellNr);
	if (m_currentSpell == -1) return;
	if (!(m_spellMap.at(m_currentSpell)->getSpellData().needsTarget) && m_currentSpell == spellNr) {
		executeCurrentSpell(sf::Vector2f());
	}
}

void SpellManager::setCurrentSpell(int spellNr) {
	if (spellNr < -1 || spellNr + 1 > static_cast<int>(m_spellMap.size())) {
		g_logger->logWarning("SpellManager::setCurrentSpell", "A invalid spell is set as current spell. Spell nr: " + to_string(spellNr));
		m_currentSpell = -1;
		return;
	}
	m_currentSpell = spellNr;
	if (m_spellSelection != nullptr) {
		m_spellSelection->selectSlot(spellNr);
	}
}

std::vector<SpellCreator*>& SpellManager::getSpellMap() {
	return m_spellMap;
}