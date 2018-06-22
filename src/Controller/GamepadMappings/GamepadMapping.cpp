#include "Controller/GamepadMappings/GamepadMapping.h"
#include "Logger.h"
#include "Enums/EnumNames.h"

GamepadMapping::GamepadMapping(const input_map& inputMap, const key_map& keyMap) {
	m_inputMap = inputMap;
	m_keyMap = keyMap;
	checkMappings();
}

void GamepadMapping::checkMappings() const {
	for (auto& key : GAMEPAD_KEYS) {
		assert(contains(m_keyMap, key));
	}

	for (auto& kv : m_keyMap) {
		auto containsAxis = false;
		for (auto& kv2 : m_inputMap) {
			if (kv2.second == kv.second) {
				containsAxis = true;
				break;
			}
		}
		
		assert(containsAxis);
	}
}

const input_map& GamepadMapping::getInputMap() const {
	return m_inputMap;
}

const key_map& GamepadMapping::getKeyMap() const {
	return m_keyMap;
}
