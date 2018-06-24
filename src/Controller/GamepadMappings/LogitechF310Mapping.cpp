#include "Controller/GamepadMappings/LogitechF310Mapping.h"

static key_map KEY_MAP() {
	return
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
}

static input_map INPUT_MAP() {
	return
	{
		{ GamepadInput::Axis_PovX_Negative, GamepadAxis::DPadLeft },
		{ GamepadInput::Axis_PovX_Positive, GamepadAxis::DPadRight },
#ifdef __linux__ 
		{ GamepadInput::Axis_PovY_Negative, GamepadAxis::DPadUp },
		{ GamepadInput::Axis_PovY_Positive, GamepadAxis::DPadDown },
#else
		{ GamepadInput::Axis_PovY_Negative, GamepadAxis::DPadDown },
		{ GamepadInput::Axis_PovY_Positive, GamepadAxis::DPadUp },
#endif 
		{ GamepadInput::Axis_X_Negative, GamepadAxis::LeftStickLeft },
		{ GamepadInput::Axis_X_Positive, GamepadAxis::LeftStickRight },
		{ GamepadInput::Axis_Y_Negative, GamepadAxis::LeftStickUp },
		{ GamepadInput::Axis_Y_Positive, GamepadAxis::LeftStickDown },
		{ GamepadInput::Axis_Z_Negative, GamepadAxis::LeftTrigger },
		{ GamepadInput::Axis_R_Negative, GamepadAxis::RightTrigger },
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
		{ GamepadInput::Button_6, GamepadAxis::Back },
		{ GamepadInput::Button_7, GamepadAxis::Start },
		{ GamepadInput::Button_8, GamepadAxis::LogitechButton },
		{ GamepadInput::Button_9, GamepadAxis::LeftButton3 },
		{ GamepadInput::Button_10, GamepadAxis::RightButton3 },
	};
}

LogitechF310Mapping::LogitechF310Mapping() : GamepadMapping(INPUT_MAP(), KEY_MAP()) {}
