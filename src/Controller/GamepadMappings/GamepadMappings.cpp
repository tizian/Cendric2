#include "Enums/EnumNames.h"
#include "ResourceManager.h"

#include "Controller/GamepadMappings/GamepadMappings.h"
#include "Controller/GamepadMappings/XboxMapping.h"
#include "Controller/GamepadMappings/LogitechF310Mapping.h"
#include "Controller/GamepadMappings/DualShock42Mapping.h"
#include "Controller/GamepadMappings/DualShock4Mapping.h"

const std::map<GamepadProductID, GamepadMapping> GamepadMappings::MAPPINGS =
{
	{GamepadProductID::XboxOne_A, XboxMapping() },
	{GamepadProductID::XboxOne_B, XboxMapping() },
	{GamepadProductID::Xbox360, XboxMapping() },
	{GamepadProductID::Xbox360_Wireless, XboxMapping() },
	{GamepadProductID::DualShock4, DualShock4Mapping() },
	{GamepadProductID::DualShock4_2, DualShock42Mapping() },
	{GamepadProductID::Logitech_F310, LogitechF310Mapping() },
};

std::map<Key, GamepadInput> GamepadMappings::getDefaultMappings(GamepadProductID id)
{
	if (contains(MAPPINGS, id)) {
		return getMappings(MAPPINGS.at(id));
	}

	return getUnknownMappings();
}

std::string GamepadMappings::getKeyName(GamepadInput input) {
	auto const id = g_resourceManager->getConfiguration().gamepadProductId;
	if (contains(MAPPINGS, id)) {
		auto const& map = MAPPINGS.at(id).getInputMap();

		if (contains(map, input)) {
			return EnumNames::getGamepadAxisName(map.at(input));
		}
	}

	return EnumNames::getGamepadInputName(input);
}

std::string GamepadMappings::getKeyName(Key key) {
	auto& keyMap = g_resourceManager->getConfiguration().gamepadKeyMap;
	auto const id = g_resourceManager->getConfiguration().gamepadProductId;

	if (contains(keyMap, key)) {
		if (contains(MAPPINGS, id)) {
			auto const& map = MAPPINGS.at(id).getInputMap();

			if (contains(map, keyMap.at(key))) {
				return EnumNames::getGamepadAxisName(map.at(keyMap.at(key)));
			}
		}

		return EnumNames::getGamepadInputName(keyMap.at(key));
	}

	return "?";
}

std::map<Key, GamepadInput> GamepadMappings::getMappings(const GamepadMapping& mapping) {
	std::map<Key, GamepadInput> mappings;

	for (auto& kv : mapping.getKeyMap())
	{
		auto potentialInput = GamepadInput::VOID;
		for (auto& kv2 : mapping.getInputMap()) {
			if (kv2.second == kv.second) {
				potentialInput = kv2.first;
				break;
			}
		}

		mappings.insert({ kv.first, potentialInput });
	}

	return mappings;
}

std::map<Key, GamepadInput> GamepadMappings::getUnknownMappings() {
	std::map<Key, GamepadInput> defaultMappings =
	{
		{ Key::Move_Up, GamepadInput::Axis_Y_Negative },
		{ Key::Move_Down, GamepadInput::Axis_Y_Positive },
		{ Key::Move_Right, GamepadInput::Axis_X_Positive },
		{ Key::Move_Left, GamepadInput::Axis_X_Positive },
		{ Key::Move_Up2, GamepadInput::Axis_PovY_Negative },
		{ Key::Move_Down2, GamepadInput::Axis_PovY_Positive },
		{ Key::Move_Right2, GamepadInput::Axis_PovX_Positive },
		{ Key::Move_Left2, GamepadInput::Axis_PovX_Negative },
		{ Key::Aim_Up, GamepadInput::Axis_V_Negative },
		{ Key::Aim_Down, GamepadInput::Axis_V_Positive },
		{ Key::Aim_Right, GamepadInput::Axis_U_Positive },
		{ Key::Aim_Left, GamepadInput::Axis_U_Negative },
		{ Key::Escape, GamepadInput::Button_3 },
		{ Key::Interact, GamepadInput::Button_2 },
		{ Key::Confirm, GamepadInput::Button_2 },
		{ Key::Jump, GamepadInput::Button_0 },
		{ Key::Attack, GamepadInput::Button_1 },
		{ Key::PreviousSpell, GamepadInput::Button_4 },
		{ Key::NextSpell, GamepadInput::Button_5 },
		{ Key::QuickSlot1, GamepadInput::Button_6 },
		{ Key::QuickSlot2, GamepadInput::Button_7 },
		{ Key::Menu, GamepadInput::Button_8 },
	};

	return defaultMappings;
}
