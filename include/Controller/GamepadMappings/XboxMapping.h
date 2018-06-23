#pragma once

#include "global.h"
#include "Controller/GamepadMappings/GamepadMapping.h"

// Xbox One and 360 do have the same mappings
class XboxMapping final : public GamepadMapping {
public:
	XboxMapping();
};