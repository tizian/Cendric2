#include "Controller/GamepadMappings/DualShock42Mapping.h"

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
		{ GamepadInput::Axis_U_Negative, GamepadAxis::RightStickLeft },
		{ GamepadInput::Axis_U_Positive, GamepadAxis::RightStickRight },
		{ GamepadInput::Axis_V_Negative, GamepadAxis::RightStickUp },
		{ GamepadInput::Axis_V_Positive, GamepadAxis::RightStickDown },
#ifdef __linux__ 
		{ GamepadInput::Button_0, GamepadAxis::X },
		{ GamepadInput::Button_1, GamepadAxis::Circle },
		{ GamepadInput::Button_2, GamepadAxis::Triangle },
		{ GamepadInput::Button_3, GamepadAxis::Square },
#else
		{ GamepadInput::Button_0, GamepadAxis::Square },
		{ GamepadInput::Button_1, GamepadAxis::X },
		{ GamepadInput::Button_2, GamepadAxis::Circle },
		{ GamepadInput::Button_3, GamepadAxis::Triangle },
#endif
		{ GamepadInput::Button_4, GamepadAxis::LeftButton1 },
		{ GamepadInput::Button_5, GamepadAxis::RightButton1 },
		{ GamepadInput::Button_6, GamepadAxis::LeftButton2 },
		{ GamepadInput::Button_7, GamepadAxis::RightButton2 },
		{ GamepadInput::Button_8, GamepadAxis::Share },
		{ GamepadInput::Button_9, GamepadAxis::Options },
		{ GamepadInput::Button_10, GamepadAxis::PSButton },
		{ GamepadInput::Button_11, GamepadAxis::LeftButton3 },
		{ GamepadInput::Button_12, GamepadAxis::RightButton3 }
	};
}

DualShock42Mapping::DualShock42Mapping() : GamepadMapping(INPUT_MAP(), KEY_MAP()) {}