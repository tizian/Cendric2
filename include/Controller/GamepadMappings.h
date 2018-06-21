#pragma once

#include "global.h"
#include "Enums/GamepadInput.h"
#include "Enums/GamepadAxis.h"
#include "Enums/GamepadProductID.h"
#include "Enums/Key.h"

typedef std::map<GamepadInput, GamepadAxis> input_map;
typedef std::map<Key, GamepadAxis> key_map;

class GamepadMappings {
public:
	static std::map<Key, GamepadInput> getDefaultMappings(GamepadProductID id);
	static std::string getKeyName(Key key);
	static std::string getKeyName(GamepadInput input);

private:
	static std::map<Key, GamepadInput> getUnknownMappings();
	static std::map<Key, GamepadInput> getMappings(const key_map& keyMap, const input_map& inputMap);

	static const input_map XboxOneInputMap;
	static const key_map XboxOneKeyMap;

	static const input_map Ds4InputMap;
	static const key_map Ds4KeyMap;

	static const input_map Ds42InputMap;
	static const key_map Ds42KeyMap;

	static const input_map LogitechF310InputMap;
	static const key_map LogitechF310KeyMap;
};