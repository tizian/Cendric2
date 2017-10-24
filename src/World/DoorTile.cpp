#include "World/DoorTile.h"
#include "CharacterCore.h"
#include "Screens/Screen.h"
#include "World/MainCharacter.h"
#include "FileIO/ParserTools.h"

void DoorTile::notifyReloadNeeded() {
	m_isReloadNeeded = true;
}

void DoorTile::initConditions(const std::map<std::string, std::string>& properties) {
	if (contains(properties, std::string("key"))) {
		m_keyItemID = properties.at("key");
	}

	if (contains(properties, std::string("not conditions"))) {
		auto notCond = ParserTools::parseConditions(properties.at("not conditions"), true);
		for (auto& cond : notCond) {
			m_conditions.push_back(cond);
		}
	}

	if (contains(properties, std::string("conditions"))) {
		auto notCond = ParserTools::parseConditions(properties.at("conditions"), false);
		for (auto& cond : notCond) {
			m_conditions.push_back(cond);
		}
	}
}

void DoorTile::reloadConditions(MainCharacter* mainChar) {
	CharacterCore* core = m_screen->getCharacterCore();

	bool conditionsFulfilled = true;
	for (auto& condition : m_conditions) {
		if ((condition.negative && core->isConditionFulfilled(condition.type, condition.name))
			|| (!condition.negative && !core->isConditionFulfilled(condition.type, condition.name))) {
			conditionsFulfilled = false;
			break;
		}
	}

	m_isConditionsFulfilled = conditionsFulfilled;

	if ((m_isConditionsFulfilled && m_keyItemID.empty() && m_strength == 0)
		|| fastIntersect(*mainChar->getBoundingBox(), *getBoundingBox())) {
		open();
	}
	else {
		close();
	}

	m_isReloadNeeded = false;
}