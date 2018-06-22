#pragma once

#include "global.h"
#include "Controller/GamepadMappings/GamepadMapping.h"
#include "Enums/GamepadProductID.h"

class GamepadMappings {
public:
	static std::map<Key, GamepadInput> getDefaultMappings(GamepadProductID id);
	static std::string getKeyName(Key key);
	static std::string getKeyName(GamepadInput input);

private:
	static std::map<Key, GamepadInput> getUnknownMappings();
	static std::map<Key, GamepadInput> getMappings(const GamepadMapping& mapping);

	static const std::map<GamepadProductID, GamepadMapping> MAPPINGS;
};
