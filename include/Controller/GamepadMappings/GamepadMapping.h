#pragma once

#include "global.h"
#include "Enums/GamepadInput.h"
#include "Enums/GamepadAxis.h"
#include "Enums/Key.h"

typedef std::map<GamepadInput, GamepadAxis> input_map;
typedef std::map<Key, GamepadAxis> key_map;

class GamepadMapping {
public:
	virtual ~GamepadMapping() = default;

	const input_map& getInputMap() const;
	const key_map& getKeyMap() const;

protected:
	GamepadMapping(const input_map& inputMap, const key_map& keyMap);

	input_map m_inputMap;
	key_map m_keyMap;

private:
	void checkMappings() const;
};