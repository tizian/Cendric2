#include "Controller/GamepadMappings.h"
#include "Enums/EnumNames.h"
#include "ResourceManager.h"

std::map<Key, GamepadInput> GamepadMappings::getDefaultMappings(GamepadProductID id)
{
	switch (id) { 

	case GamepadProductID::XboxOne_A: 
	case GamepadProductID::XboxOne_B: 
		return getMappings(XboxOneKeyMap, XboxOneInputMap);

	case GamepadProductID::Dualshock4: 
		return getMappings(Ds4KeyMap, Ds4InputMap);

	default:
		return getUnknownMappings();
	}
}

std::string GamepadMappings::getKeyName(GamepadInput input) {
	switch (g_resourceManager->getConfiguration().gamepadProductId) {
	case GamepadProductID::XboxOne_A:
	case GamepadProductID::XboxOne_B:
		if (contains(XboxOneInputMap, input)) {
			return EnumNames::getGamepadAxisName(XboxOneInputMap.at(input));
		}
		break;

	case GamepadProductID::Dualshock4:
		if (contains(Ds4InputMap, input)) {
			return EnumNames::getGamepadAxisName(Ds4InputMap.at(input));
		}
		break;
	default:
		break;
	}
	
	return EnumNames::getGamepadInputName(input);
}

std::string GamepadMappings::getKeyName(Key key) {
	auto& keyMap = g_resourceManager->getConfiguration().gamepadKeyMap;

	switch (g_resourceManager->getConfiguration().gamepadProductId) {
	case GamepadProductID::XboxOne_A:
	case GamepadProductID::XboxOne_B:
		if (contains(XboxOneKeyMap, key)) {
			return EnumNames::getGamepadAxisName(XboxOneKeyMap.at(key));
		}
		break;
		
	case GamepadProductID::Dualshock4:
		if (contains(Ds4KeyMap, key)) {
			return EnumNames::getGamepadAxisName(Ds4KeyMap.at(key));
		}
		break;
	default:
		break;
	}

	if (contains(keyMap, key)) {
		return EnumNames::getGamepadInputName(keyMap.at(key));
	}

	return "?";
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

std::map<Key, GamepadInput> GamepadMappings::getMappings(const key_map& keyMap, const input_map& inputMap) {
	std::map<Key, GamepadInput> mappings;
	
	for (auto& kv : keyMap)
	{
		auto potentialInput = GamepadInput::VOID;
		for (auto& kv2 : inputMap) {
			if (kv2.second == kv.second) {
				potentialInput = kv2.first;
				break;
			}
		}

		mappings.insert({ kv.first, potentialInput });
	}

	return mappings;
}

const key_map GamepadMappings::XboxOneKeyMap =
{
	{ Key::Move_Up, GamepadAxis::LeftStickUp },
	{ Key::Move_Down, GamepadAxis::LeftStickDown },
	{ Key::Move_Right, GamepadAxis::LeftStickRight },
	{ Key::Move_Left, GamepadAxis::LeftStickLeft },
	{ Key::Move_Up2, GamepadAxis::DPadUp },
	{ Key::Move_Down2, GamepadAxis::DPadDown },
	{ Key::Move_Right2, GamepadAxis::DPadRight },
	{ Key::Move_Left2, GamepadAxis::DPadLeft },
	{ Key::Aim_Up, GamepadAxis::RightStickUp },
	{ Key::Aim_Down, GamepadAxis::RightStickDown },
	{ Key::Aim_Right, GamepadAxis::RightStickRight },
	{ Key::Aim_Left, GamepadAxis::RightStickLeft },
	{ Key::Escape, GamepadAxis::Y },
	{ Key::Interact, GamepadAxis::X },
	{ Key::Confirm, GamepadAxis::X },
	{ Key::Jump, GamepadAxis::A },
	{ Key::Attack, GamepadAxis::B },
	{ Key::PreviousSpell, GamepadAxis::LeftShoulder },
	{ Key::NextSpell, GamepadAxis::RightShoulder },
	{ Key::QuickSlot1, GamepadAxis::LeftTrigger },
	{ Key::QuickSlot2, GamepadAxis::RightTrigger },
	{ Key::Menu, GamepadAxis::Start },
};

const input_map GamepadMappings::XboxOneInputMap =
{
	{ GamepadInput::Axis_PovX_Negative, GamepadAxis::DPadLeft },
	{ GamepadInput::Axis_PovX_Positive, GamepadAxis::DPadRight },
	{ GamepadInput::Axis_PovY_Negative, GamepadAxis::DPadDown },
	{ GamepadInput::Axis_PovY_Positive, GamepadAxis::DPadUp },
	{ GamepadInput::Axis_X_Negative, GamepadAxis::LeftStickLeft },
	{ GamepadInput::Axis_X_Positive, GamepadAxis::LeftStickRight },
	{ GamepadInput::Axis_Y_Negative, GamepadAxis::LeftStickUp },
	{ GamepadInput::Axis_Y_Positive, GamepadAxis::LeftStickDown },
	{ GamepadInput::Axis_Z_Negative, GamepadAxis::RightTrigger },
	{ GamepadInput::Axis_Z_Positive, GamepadAxis::LeftTrigger },
	{ GamepadInput::Axis_V_Negative, GamepadAxis::RightStickUp },
	{ GamepadInput::Axis_V_Positive, GamepadAxis::RightStickDown },
	{ GamepadInput::Axis_U_Negative, GamepadAxis::RightStickLeft },
	{ GamepadInput::Axis_U_Positive, GamepadAxis::RightStickRight },
	{ GamepadInput::Button_0, GamepadAxis::A },
	{ GamepadInput::Button_1, GamepadAxis::B },
	{ GamepadInput::Button_2, GamepadAxis::X },
	{ GamepadInput::Button_3, GamepadAxis::Y },
	{ GamepadInput::Button_4, GamepadAxis::LeftShoulder },
	{ GamepadInput::Button_5, GamepadAxis::RightShoulder },
	{ GamepadInput::Button_6, GamepadAxis::Select },
	{ GamepadInput::Button_7, GamepadAxis::Start },
	{ GamepadInput::Button_8, GamepadAxis::LeftStickPush },
	{ GamepadInput::Button_9, GamepadAxis::RightStickPush },
};

const input_map GamepadMappings::Ds4InputMap =
{
	{ GamepadInput::Axis_PovX_Negative, GamepadAxis::DPadLeft },
	{ GamepadInput::Axis_PovX_Positive, GamepadAxis::DPadRight },
	{ GamepadInput::Axis_PovY_Negative, GamepadAxis::DPadDown },
	{ GamepadInput::Axis_PovY_Positive, GamepadAxis::DPadUp },
	{ GamepadInput::Axis_X_Negative, GamepadAxis::LeftStickLeft },
	{ GamepadInput::Axis_X_Positive, GamepadAxis::LeftStickRight },
	{ GamepadInput::Axis_Y_Negative, GamepadAxis::LeftStickUp },
	{ GamepadInput::Axis_Y_Positive, GamepadAxis::LeftStickDown },
	{ GamepadInput::Axis_Z_Negative, GamepadAxis::RightStickLeft },
	{ GamepadInput::Axis_Z_Positive, GamepadAxis::RightStickRight },
	{ GamepadInput::Axis_R_Negative, GamepadAxis::RightStickUp },
	{ GamepadInput::Axis_R_Positive, GamepadAxis::RightStickDown },
	{ GamepadInput::Button_0, GamepadAxis::Square },
	{ GamepadInput::Button_1, GamepadAxis::X },
	{ GamepadInput::Button_2, GamepadAxis::Circle },
	{ GamepadInput::Button_3, GamepadAxis::Triangle },
	{ GamepadInput::Button_4, GamepadAxis::LeftButton1 },
	{ GamepadInput::Button_5, GamepadAxis::RightButton1 },
	{ GamepadInput::Button_6, GamepadAxis::LeftButton2 },
	{ GamepadInput::Button_7, GamepadAxis::RightButton2 },
	{ GamepadInput::Button_8, GamepadAxis::Share },
	{ GamepadInput::Button_9, GamepadAxis::Options },
	{ GamepadInput::Button_10, GamepadAxis::LeftStickPush },
	{ GamepadInput::Button_11, GamepadAxis::RightStickPush },
	{ GamepadInput::Button_12, GamepadAxis::PSButton },
	{ GamepadInput::Button_13, GamepadAxis::Touchpad },
};

const std::map<Key, GamepadAxis> GamepadMappings::Ds4KeyMap =
{
	{ Key::Move_Up, GamepadAxis::LeftStickUp },
	{ Key::Move_Down, GamepadAxis::LeftStickDown },
	{ Key::Move_Right, GamepadAxis::LeftStickRight },
	{ Key::Move_Left, GamepadAxis::LeftStickLeft },
	{ Key::Move_Up2, GamepadAxis::DPadUp },
	{ Key::Move_Down2, GamepadAxis::DPadDown },
	{ Key::Move_Right2, GamepadAxis::DPadRight },
	{ Key::Move_Left2, GamepadAxis::DPadLeft },
	{ Key::Aim_Up, GamepadAxis::RightStickUp },
	{ Key::Aim_Down, GamepadAxis::RightStickDown },
	{ Key::Aim_Right, GamepadAxis::RightStickRight },
	{ Key::Aim_Left, GamepadAxis::RightStickLeft },
	{ Key::Escape, GamepadAxis::Triangle },
	{ Key::Interact, GamepadAxis::Square },
	{ Key::Confirm, GamepadAxis::Square },
	{ Key::Jump, GamepadAxis::X },
	{ Key::Attack, GamepadAxis::Circle },
	{ Key::PreviousSpell, GamepadAxis::LeftButton1 },
	{ Key::NextSpell, GamepadAxis::RightButton1 },
	{ Key::QuickSlot1, GamepadAxis::LeftButton2 },
	{ Key::QuickSlot2, GamepadAxis::RightButton2 },
	{ Key::Menu, GamepadAxis::Options },
};