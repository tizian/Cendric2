#include "Controller/GamepadController.h"
#include "Enums/GamepadProductID.h"
#include "ResourceManager.h"

const int GamepadController::AXIS_THRESHOLD = 50;

GamepadController::GamepadController() {
	m_joystickMap = &(g_resourceManager->getConfiguration().gamepadKeyMap);
};

void GamepadController::update(const sf::Time& frameTime) {
	if (!isGamepadConnected()) return;
	m_lastPressedInput = GamepadInput::VOID;
	updateLeftJoystick();
}

bool GamepadController::isGamepadConnected() const {
	return m_connectedJoystick > -1;
}

void GamepadController::updateLeftJoystick() {
	const auto left = isGamepadButtonPressed(Key::Move_Left) || isGamepadButtonPressed(Key::Move_Left2);
	m_isLeftJoystickLeftJustPressed = !m_isLeftJoystickLeftPressed && left;
	m_isLeftJoystickLeftPressed = left;

	const auto right = isGamepadButtonPressed(Key::Move_Right) || isGamepadButtonPressed(Key::Move_Right2);
	m_isLeftJoystickRightJustPressed = !m_isLeftJoystickRightPressed && right;
	m_isLeftJoystickRightPressed = right;

	const auto up = isGamepadButtonPressed(Key::Move_Up) || isGamepadButtonPressed(Key::Move_Up2);
	m_isLeftJoystickUpJustPressed = !m_isLeftJoystickUpPressed && up;
	m_isLeftJoystickUpPressed = up;

	const auto down = isGamepadButtonPressed(Key::Move_Down) || isGamepadButtonPressed(Key::Move_Down2);
	m_isLeftJoystickDownJustPressed = !m_isLeftJoystickDownPressed && down;
	m_isLeftJoystickDownPressed = down;
}

bool GamepadController::isGamepadButtonPressed(Key key) const {
	auto const it = m_joystickMap->find(key);
	if (it == m_joystickMap->end()) {
		return false;
	}

	return isGamepadInputPressed((*it).second);
}

std::pair<sf::Joystick::Axis, bool> GamepadController::getAxisForInput(GamepadInput input)
{
	std::pair<sf::Joystick::Axis, bool> pair = { static_cast<sf::Joystick::Axis>(sf::Joystick::AxisCount), false };
	switch (input)
	{
	case GamepadInput::Axis_X_Positive: 
		pair.first = sf::Joystick::X;
		pair.second = true;
		break;
	case GamepadInput::Axis_X_Negative:
		pair.first = sf::Joystick::X;
		pair.second = false;
		break;
	case GamepadInput::Axis_Y_Positive:
		pair.first = sf::Joystick::Y;
		pair.second = true;
		break;
	case GamepadInput::Axis_Y_Negative: 
		pair.first = sf::Joystick::Y;
		pair.second = false;
		break;
	case GamepadInput::Axis_Z_Positive: 
		pair.first = sf::Joystick::Z;
		pair.second = true;
		break;
	case GamepadInput::Axis_Z_Negative: 
		pair.first = sf::Joystick::Z;
		pair.second = false;
		break;
	case GamepadInput::Axis_R_Positive: 
		pair.first = sf::Joystick::R;
		pair.second = true;
		break;
	case GamepadInput::Axis_R_Negative: 
		pair.first = sf::Joystick::R;
		pair.second = false;
		break;
	case GamepadInput::Axis_U_Positive: 
		pair.first = sf::Joystick::U;
		pair.second = true;
		break;
	case GamepadInput::Axis_U_Negative: 
		pair.first = sf::Joystick::U;
		pair.second = false;
		break;
	case GamepadInput::Axis_V_Positive: 
		pair.first = sf::Joystick::V;
		pair.second = true;
		break;
	case GamepadInput::Axis_V_Negative: 
		pair.first = sf::Joystick::V;
		pair.second = false;
		break;
	case GamepadInput::Axis_PovX_Positive: 
		pair.first = sf::Joystick::PovX;
		pair.second = true;
		break;
	case GamepadInput::Axis_PovX_Negative: 
		pair.first = sf::Joystick::PovX;
		pair.second = false;
		break;
	case GamepadInput::Axis_PovY_Positive: 
		pair.first = sf::Joystick::PovY;
		pair.second = true;
		break;
	case GamepadInput::Axis_PovY_Negative: 
		pair.first = sf::Joystick::PovY;
		pair.second = false;
		break;
	default: break;
	}

	return pair;
}

bool GamepadController::isGamepadInputPressed(GamepadInput input) const {
	if (!isGamepadConnected()) {
		return false;
	}

	if (input <= GamepadInput::VOID || input >= GamepadInput::MAX) {
		return false;
	}

	switch (input) { 
	default: return false;
	case GamepadInput::Axis_X_Positive: return isAxisPositive(sf::Joystick::X);
	case GamepadInput::Axis_X_Negative: return isAxisNegative(sf::Joystick::X);
	case GamepadInput::Axis_Y_Positive: return isAxisPositive(sf::Joystick::Y);
	case GamepadInput::Axis_Y_Negative: return isAxisNegative(sf::Joystick::Y);
	case GamepadInput::Axis_Z_Positive: return isAxisPositive(sf::Joystick::Z);
	case GamepadInput::Axis_Z_Negative: return isAxisNegative(sf::Joystick::Z);
	case GamepadInput::Axis_R_Positive: return isAxisPositive(sf::Joystick::R);
	case GamepadInput::Axis_R_Negative: return isAxisNegative(sf::Joystick::R);
	case GamepadInput::Axis_U_Positive: return isAxisPositive(sf::Joystick::U);
	case GamepadInput::Axis_U_Negative: return isAxisNegative(sf::Joystick::U);
	case GamepadInput::Axis_V_Positive: return isAxisPositive(sf::Joystick::V);
	case GamepadInput::Axis_V_Negative: return isAxisNegative(sf::Joystick::V);
	case GamepadInput::Axis_PovX_Positive: return isAxisPositive(sf::Joystick::PovX);
	case GamepadInput::Axis_PovX_Negative: return isAxisNegative(sf::Joystick::PovX);
	case GamepadInput::Axis_PovY_Positive: return isAxisPositive(sf::Joystick::PovY);
	case GamepadInput::Axis_PovY_Negative: return isAxisNegative(sf::Joystick::PovY);
	case GamepadInput::Button_0: return isButtonPressed(0);
	case GamepadInput::Button_1: return isButtonPressed(1);
	case GamepadInput::Button_2: return isButtonPressed(2);
	case GamepadInput::Button_3: return isButtonPressed(3);
	case GamepadInput::Button_4: return isButtonPressed(4);
	case GamepadInput::Button_5: return isButtonPressed(5);
	case GamepadInput::Button_6: return isButtonPressed(6);
	case GamepadInput::Button_7: return isButtonPressed(7);
	case GamepadInput::Button_8: return isButtonPressed(8);
	case GamepadInput::Button_9: return isButtonPressed(9);
	case GamepadInput::Button_10: return isButtonPressed(10);
	case GamepadInput::Button_11: return isButtonPressed(11);
	case GamepadInput::Button_12: return isButtonPressed(12);
	case GamepadInput::Button_13: return isButtonPressed(13);
	case GamepadInput::Button_14: return isButtonPressed(14);
	case GamepadInput::Button_15: return isButtonPressed(15);
	case GamepadInput::Button_16: return isButtonPressed(16);
	case GamepadInput::Button_17: return isButtonPressed(17);
	case GamepadInput::Button_18: return isButtonPressed(18);
	case GamepadInput::Button_19: return isButtonPressed(19);
	case GamepadInput::Button_20: return isButtonPressed(20);
	case GamepadInput::Button_21: return isButtonPressed(21);
	case GamepadInput::Button_22: return isButtonPressed(22);
	case GamepadInput::Button_23: return isButtonPressed(23);
	case GamepadInput::Button_24: return isButtonPressed(24);
	case GamepadInput::Button_25: return isButtonPressed(25);
	case GamepadInput::Button_26: return isButtonPressed(26);
	case GamepadInput::Button_27: return isButtonPressed(27);
	case GamepadInput::Button_28: return isButtonPressed(28);
	case GamepadInput::Button_29: return isButtonPressed(29);
	case GamepadInput::Button_30: return isButtonPressed(30);
	case GamepadInput::Button_31: return isButtonPressed(31);
	}
}

bool GamepadController::isAxisNegative(sf::Joystick::Axis axis) const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, axis) < -AXIS_THRESHOLD;
}

bool GamepadController::isAxisPositive(sf::Joystick::Axis axis) const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, axis) > AXIS_THRESHOLD;
}

bool GamepadController::isButtonPressed(int button) const {
	return sf::Joystick::isButtonPressed(m_connectedJoystick, button);
}

bool GamepadController::isLeftJoystickUp() const {
	return m_isLeftJoystickUpPressed;
}

bool GamepadController::isLeftJoystickDown() const {
	return m_isLeftJoystickDownPressed;
}

bool GamepadController::isLeftJoystickLeft() const {
	return m_isLeftJoystickLeftPressed;
}

bool GamepadController::isLeftJoystickRight() const {
	return m_isLeftJoystickRightPressed;
}

bool GamepadController::isLeftJoystickJustUp() const {
	return m_isLeftJoystickUpJustPressed;
}

bool GamepadController::isLeftJoystickJustDown() const {
	return m_isLeftJoystickDownJustPressed;
}

bool GamepadController::isLeftJoystickJustLeft() const {
	return m_isLeftJoystickLeftJustPressed;
}

bool GamepadController::isLeftJoystickJustRight() const {
	return m_isLeftJoystickRightJustPressed;
}

sf::Vector2f GamepadController::getAnyMoveJoystickAxis() const {
	sf::Vector2f axis = getAxisPosition(Key::Move_Right, Key::Move_Down);
	if (norm(axis) > AXIS_THRESHOLD) {
		return axis;
	}

	return getAxisPosition(Key::Move_Right2, Key::Move_Down2);
}

sf::Vector2f GamepadController::getAimJoystickAxis() const {
	return getAxisPosition(Key::Aim_Right, Key::Aim_Down);
}

sf::Vector2f GamepadController::getAxisPosition(Key keyRight, Key keyDown) const {
	if (!isGamepadConnected()) {
		return {};
	}

	auto const xAxis = getAxisForInput(getGamepadInputForKey(keyRight));
	auto const yAxis = getAxisForInput(getGamepadInputForKey(keyDown));

	auto const signX = xAxis.second ? 1 : -1;
	auto const signY = yAxis.second ? 1 : -1;

	sf::Vector2f axis;
	axis.x = signX * sf::Joystick::getAxisPosition(m_connectedJoystick, xAxis.first);
	axis.y = signY * sf::Joystick::getAxisPosition(m_connectedJoystick, yAxis.first);

	return axis;
}

GamepadInput GamepadController::getGamepadInputForKey(Key key) const {
	auto const it = m_joystickMap->find(key);
	if (it == m_joystickMap->end()) {
		return GamepadInput::VOID;
	}

	return (*it).second;
}

void GamepadController::setLastPressedGamepadAxis(sf::Event::JoystickMoveEvent event) {
	if (static_cast<int>(event.joystickId) != m_connectedJoystick) {
		return;
	}

	if (std::abs(event.position) < AXIS_THRESHOLD) {
		return;
	}

	const auto negative = event.position < 0;

	switch (event.axis)
	{
	case sf::Joystick::X: m_lastPressedInput = negative ? GamepadInput::Axis_X_Negative : GamepadInput::Axis_X_Positive; break;
	case sf::Joystick::Y: m_lastPressedInput = negative ? GamepadInput::Axis_Y_Negative : GamepadInput::Axis_Y_Positive; break;
	case sf::Joystick::Z: m_lastPressedInput = negative ? GamepadInput::Axis_Z_Negative : GamepadInput::Axis_Z_Positive; break;
	case sf::Joystick::R: m_lastPressedInput = negative ? GamepadInput::Axis_R_Negative : GamepadInput::Axis_R_Positive; break;
	case sf::Joystick::U: m_lastPressedInput = negative ? GamepadInput::Axis_U_Negative : GamepadInput::Axis_U_Positive; break;
	case sf::Joystick::V: m_lastPressedInput = negative ? GamepadInput::Axis_V_Negative : GamepadInput::Axis_V_Positive; break;
	case sf::Joystick::PovX: m_lastPressedInput = negative ? GamepadInput::Axis_PovX_Negative : GamepadInput::Axis_PovX_Positive; break;
	case sf::Joystick::PovY: m_lastPressedInput = negative ? GamepadInput::Axis_PovY_Negative : GamepadInput::Axis_PovY_Positive; break;
	}
}

void GamepadController::setLastPressedGamepadButton(sf::Event::JoystickButtonEvent event) {
	if (static_cast<int>(event.joystickId) != m_connectedJoystick) {
		return;
	}

	switch (event.button)
	{
	default: m_lastPressedInput = GamepadInput::VOID; break;
	case 0: m_lastPressedInput = GamepadInput::Button_0; break;
	case 1: m_lastPressedInput = GamepadInput::Button_1; break;
	case 2: m_lastPressedInput = GamepadInput::Button_2; break;
	case 3: m_lastPressedInput = GamepadInput::Button_3; break;
	case 4: m_lastPressedInput = GamepadInput::Button_4; break;
	case 5: m_lastPressedInput = GamepadInput::Button_5; break;
	case 6: m_lastPressedInput = GamepadInput::Button_6; break;
	case 7: m_lastPressedInput = GamepadInput::Button_7; break;
	case 8: m_lastPressedInput = GamepadInput::Button_8; break;
	case 9: m_lastPressedInput = GamepadInput::Button_9; break;
	case 10: m_lastPressedInput = GamepadInput::Button_10; break;
	case 11: m_lastPressedInput = GamepadInput::Button_11; break;
	case 12: m_lastPressedInput = GamepadInput::Button_12; break;
	case 13: m_lastPressedInput = GamepadInput::Button_13; break;
	case 14: m_lastPressedInput = GamepadInput::Button_14; break;
	case 15: m_lastPressedInput = GamepadInput::Button_15; break;
	case 16: m_lastPressedInput = GamepadInput::Button_16; break;
	case 17: m_lastPressedInput = GamepadInput::Button_17; break;
	case 18: m_lastPressedInput = GamepadInput::Button_18; break;
	case 19: m_lastPressedInput = GamepadInput::Button_19; break;
	case 20: m_lastPressedInput = GamepadInput::Button_20; break;
	case 21: m_lastPressedInput = GamepadInput::Button_21; break;
	case 22: m_lastPressedInput = GamepadInput::Button_22; break;
	case 23: m_lastPressedInput = GamepadInput::Button_23; break;
	case 24: m_lastPressedInput = GamepadInput::Button_24; break;
	case 25: m_lastPressedInput = GamepadInput::Button_25; break;
	case 26: m_lastPressedInput = GamepadInput::Button_26; break;
	case 27: m_lastPressedInput = GamepadInput::Button_27; break;
	case 28: m_lastPressedInput = GamepadInput::Button_28; break;
	case 29: m_lastPressedInput = GamepadInput::Button_29; break;
	case 30: m_lastPressedInput = GamepadInput::Button_30; break;
	case 31: m_lastPressedInput = GamepadInput::Button_31; break;
	}
}

GamepadInput GamepadController::getLastPressedGamepadInput() const {
	return m_lastPressedInput;
}

void GamepadController::notifyGamepadConnected() {
	m_connectedJoystick = -1;
	for (int i = 0; i < 8; i++) {
		if (sf::Joystick::isConnected(i)) {
			m_connectedJoystick = i;
			auto productId = static_cast<GamepadProductID>(sf::Joystick::getIdentification(i).productId);

			if (productId != g_resourceManager->getConfiguration().gamepadProductId) {
				g_resourceManager->getConfiguration().reloadGamepadMapping(productId);
			}

			// TODO: should only be info.
			g_logger->logError("GamepadController", "Gamepad Product ID: " + std::to_string(static_cast<int>(productId)));

			break;
		}
	}
}

GamepadProductID GamepadController::getCurrentGamepadProductId() {
	auto id = GamepadProductID::Unknown;
	for (int i = 0; i < 8; i++) {
		if (sf::Joystick::isConnected(i)) {
			id = static_cast<GamepadProductID>(sf::Joystick::getIdentification(i).productId);
			break;
		}
	}

	return id;
}

