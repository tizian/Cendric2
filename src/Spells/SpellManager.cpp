#include "Spells/SpellManager.h"
#include "GUI/SpellSelection.h"

SpellManager::SpellManager(LevelMovableGameObject* owner) {
	m_currentSpell = -1;
	m_owner = owner;
}

SpellManager::~SpellManager() {
	clearSpells();
}

void SpellManager::clearSpells() {
	CLEAR_VECTOR(m_spellMap);
	m_currentSpell = -1;
	if (m_spellSelection) {
		m_spellSelection->reload();
	}
}

sf::Time SpellManager::getCooldown(SpellID id) {
	if (!contains(m_coolDownMap, id)) {
		return sf::Time::Zero;
	}
	return m_coolDownMap.at(id);
}

int SpellManager::getSelectedSpellID() const {
	return m_currentSpell;
}

SpellData* SpellManager::getSelectedSpell() const {
	return getSpell(m_currentSpell);
}

SpellData* SpellManager::getSpell(int spellId) const {
	if (spellId < 0 || spellId > static_cast<int>(m_spellMap.size()) - 1) {
		return nullptr;
	}
	return &m_spellMap.at(spellId)->getSpellData();
}

LevelMovableGameObject* SpellManager::getOwner() const {
	return m_owner;
}

void SpellManager::setSpellSelection(SpellSelection* selection) {
	m_spellSelection = selection;
}

void SpellManager::setSpellsAllied(bool value) {
	for (auto& spellcreator : m_spellMap) {
		spellcreator->setSpellAllied(value);
	}
}

void SpellManager::setGlobalCooldown(const sf::Time& cooldown) {
	m_globalCooldown = cooldown;
}

void SpellManager::setInitialCooldown(const sf::Time& cooldown, SpellID id) {
	m_coolDownMap[id] = cooldown;
}

void SpellManager::addSpell(const SpellData& spell) {
	addSpell(spell, std::vector<SpellModifier>());
}

void SpellManager::addSpell(const SpellData& spell, const std::vector<SpellModifier>& modifiers) {
	m_spellMap.push_back(SpellData::getSpellCreator(spell, modifiers, m_owner));
	if (!contains(m_coolDownMap, spell.id)) {
		m_coolDownMap.insert({ spell.id, sf::Time::Zero });
	}
	if (m_spellSelection) {
		m_spellSelection->reload();
	}
}

template <typename T>
bool SpellManager::_executeCurrentSpell(T target, bool force) {
	if (m_currentSpell < 0) return false;
	auto data = m_spellMap[m_currentSpell]->getSpellData();
	if (!force) {
		// check if execution is ready.
		if (m_remainingGlobalCooldown.asMilliseconds() != 0) return false;
		if (m_currentSpell == -1 || m_coolDownMap[data.id].asMilliseconds() != 0) return false;
		if (!m_owner->isReady()) return false;
		for (auto& spellcreator : m_spellMap) {
			if (!spellcreator->isReady())
				return false;
		}
	}

	// spell has been cast. set cooldown.
	sf::Time cooldown = m_spellMap[m_currentSpell]->getSpellData().cooldown * m_owner->getAttributes()->cooldownMultiplier;
	if (m_spellSelection &&  m_spellMap[m_currentSpell]->getSpellData().attachedToMob) {
		cooldown = std::max(m_spellMap[m_currentSpell]->getSpellData().activeDuration, cooldown);
	}
	m_coolDownMap[data.id] = cooldown;
	m_remainingGlobalCooldown = m_globalCooldown;
	m_spellMap[m_currentSpell]->executeSpell(target);
	if (m_spellSelection != nullptr) {
		m_spellSelection->activateSlot(m_currentSpell, cooldown);
	}
	return true;
}

bool SpellManager::executeCurrentSpell(const LevelMovableGameObject* target, bool force) {
	return _executeCurrentSpell<const LevelMovableGameObject*>(target, force);
}

bool SpellManager::executeCurrentSpell(const sf::Vector2f& target, bool force) {
	return _executeCurrentSpell<const sf::Vector2f&>(target, force);
}

void SpellManager::update(sf::Time frameTime) {
	// update global cooldown
	updateTime(m_remainingGlobalCooldown, frameTime);
	// update cooldown map
	for (auto& it : m_coolDownMap) {
		updateTime(it.second, frameTime);
	}
	// update spellcreators
	for (auto& it : m_spellMap) {
		(*it).update(frameTime);
	}
}

void SpellManager::setAndExecuteSpell(int spellNr) {
	if (!setCurrentSpell(spellNr)) return;
	if (!(m_spellMap.at(m_currentSpell)->getSpellData().needsTarget) && m_currentSpell == spellNr) {
		executeCurrentSpell(sf::Vector2f());
	}
}

bool SpellManager::setCurrentSpell(int spellNr) {
	if (spellNr < 0 || spellNr > static_cast<int>(m_spellMap.size() - 1)) {
		g_logger->logInfo("SpellManager::setCurrentSpell", "A invalid spell is set as current spell. Spell nr: " + std::to_string(spellNr));
		m_currentSpell = -1;
		return false;
	}
	if (m_spellSelection != nullptr && m_spellSelection->getSlots()[spellNr].isLocked()) {
		g_logger->logInfo("SpellManager::setCurrentSpell", "This spell is locked. Spell nr: " + std::to_string(spellNr));
		m_currentSpell = -1;
		return false;
	}
	m_currentSpell = spellNr;
	if (m_spellSelection != nullptr) {
		m_spellSelection->selectSlot(spellNr);
	}
	return true;
}

std::vector<SpellCreator*>& SpellManager::getSpellMap() {
	return m_spellMap;
}