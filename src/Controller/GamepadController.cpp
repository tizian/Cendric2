#include "Controller/GamepadController.h"
#include "Enums/GamepadProductID.h"
#include "ResourceManager.h"

const int GamepadController::AXIS_THRESHOLD = 50;

GamepadController::GamepadController() {
	m_joystickMap = &(g_resourceManager->getConfiguration().gamepadKeyMap);
	initAxisMap();
};

void GamepadController::update(const sf::Time& frameTime) {
	if (!isGamepadConnected()) return;
	m_lastPressedAxis = GamepadAxis::VOID;
	updateLeftJoystick();
}

bool GamepadController::isGamepadConnected() const {
	return m_connectedJoystick > -1;
}

bool GamepadController::isXboxControllerConnected() const {
	return m_isXBoxController;
}

void GamepadController::updateLeftJoystick() {
	bool left = isLeftStickLeft() || isDPadLeft();
	m_isLeftJoystickLeftJustPressed = !m_isLeftJoystickLeftPressed && left;
	m_isLeftJoystickLeftPressed = left;

	bool right = isLeftStickRight() || isDPadRight();
	m_isLeftJoystickRightJustPressed = !m_isLeftJoystickRightPressed && right;
	m_isLeftJoystickRightPressed = right;

	bool up = isLeftStickUp() || isDPadUp();
	m_isLeftJoystickUpJustPressed = !m_isLeftJoystickUpPressed && up;
	m_isLeftJoystickUpPressed = up;

	bool down = isLeftStickDown() || isDPadDown();
	m_isLeftJoystickDownJustPressed = !m_isLeftJoystickDownPressed && down;
	m_isLeftJoystickDownPressed = down;
}

bool GamepadController::isGamepadButtonPressed(Key key) const {
	auto const it = m_joystickMap->find(key);
	if (it == m_joystickMap->end()) {
		return false;
	}

	return isGamepadAxisPressed((*it).second);
}

bool GamepadController::isGamepadAxisPressed(GamepadAxis axis) const {
	if (!isGamepadConnected()) {
		return false;
	}
	if (axis <= GamepadAxis::VOID || axis >= GamepadAxis::MAX) {
		return false;
	}
	return m_axisMap.at(axis)();
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

sf::Vector2f GamepadController::getRightJoystickAxis() const {
	sf::Vector2f axis;
	if (m_isXBoxController) {
		axis.x = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::U);
		axis.y = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::V);
	}
	else {
		axis.x = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Z);
		axis.y = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::R);
	}
	
	return axis;
}

GamepadAxis GamepadController::getGamepadAxisForKey(Key key) const {
	auto const it = m_joystickMap->find(key);
	if (it == m_joystickMap->end()) {
		return GamepadAxis::VOID;
	}

	return (*it).second;
}

void GamepadController::setLastPressedGamepadAxis(sf::Event::JoystickMoveEvent event) {
	if (event.joystickId != m_connectedJoystick) {
		return;
	}

	if (std::abs(event.position) < AXIS_THRESHOLD) {
		return;
	}

	if (isXboxControllerConnected()) {
		m_lastPressedAxis = getLastPressedGamepadAxisXbox(event.axis, event.position < 0);
	}
	else {
		m_lastPressedAxis = getLastPressedGamepadAxisDs4(event.axis, event.position < 0);
	}
}

void GamepadController::setLastPressedGamepadButton(sf::Event::JoystickButtonEvent event) {
	if (event.joystickId != m_connectedJoystick) {
		return;
	}

	if (isXboxControllerConnected()) {
		m_lastPressedAxis = getLastPressedGamepadButtonXbox(event.button);
	}
	else {
		m_lastPressedAxis = getLastPressedGamepadButtonDs4(event.button);
	}
}

GamepadAxis GamepadController::getLastPressedAxis() const {
	return m_lastPressedAxis;
}

void GamepadController::notifyGamepadConnected() {
	m_connectedJoystick = -1;
	for (int i = 0; i < 8; i++) {
		if (sf::Joystick::isConnected(i)) {
			m_connectedJoystick = i;
			auto productId = sf::Joystick::getIdentification(i).productId;
			m_isXBoxController = 
				productId == static_cast<int>(GamepadProductID::XBoxOne_A) || 
				productId == static_cast<int>(GamepadProductID::XBoxOne_B);
			break;
		}
	}
}

bool GamepadController::isDPadUp() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovY) > AXIS_THRESHOLD;
}

bool GamepadController::isDPadDown() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovY) < -AXIS_THRESHOLD;
}

bool GamepadController::isDPadLeft() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovX) < -AXIS_THRESHOLD;
}

bool GamepadController::isDPadRight() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovX) > AXIS_THRESHOLD;
}

bool GamepadController::isLeftStickUp() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Y) < -AXIS_THRESHOLD;
}

bool GamepadController::isLeftStickDown() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Y) > AXIS_THRESHOLD;
}

bool GamepadController::isLeftStickLeft() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::X) < -AXIS_THRESHOLD;
}

bool GamepadController::isLeftStickRight() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::X) > AXIS_THRESHOLD;
}

bool GamepadController::isRightStickUp() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::R) < -AXIS_THRESHOLD;
}

bool GamepadController::isRightStickDown() const {
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::R) > AXIS_THRESHOLD;
}

bool GamepadController::isRightStickLeft() const {
	if (m_isXBoxController) {
		return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::U) < -AXIS_THRESHOLD;
	}
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Z) < -AXIS_THRESHOLD;
}

bool GamepadController::isRightStickRight() const {
	if (m_isXBoxController) {
		return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::U) > AXIS_THRESHOLD;
	}
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Z) > AXIS_THRESHOLD;
}

bool GamepadController::isLeftTrigger() const {
	if (m_isXBoxController) {
		return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Z) > AXIS_THRESHOLD;
	}
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::V) > AXIS_THRESHOLD;
}

bool GamepadController::isRightTrigger() const {
	if (m_isXBoxController) {
		return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Z) < -AXIS_THRESHOLD;
	}
	return sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::U) > AXIS_THRESHOLD;
}

bool GamepadController::isSquare() const {
	if (m_isXBoxController) {
		return false;
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 0);
}

bool GamepadController::isCircle() const {
	if (m_isXBoxController) {
		return false;
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 2);
}

bool GamepadController::isTriangle() const {
	if (m_isXBoxController) {
		return false;
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 3);
}

bool GamepadController::isX() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 2);
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 1);
}

bool GamepadController::isY() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 3);
	}
	return false;
}

bool GamepadController::isA() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 0);
	}
	return false;
}

bool GamepadController::isB() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 1);
	}
	return false;
}

bool GamepadController::isLeftShoulder() const {
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 4);
}

bool GamepadController::isRightShoulder() const {
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 5);
}

bool GamepadController::isSelect() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 6);
	}
	return false;
}

bool GamepadController::isStart() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 7);
	}
	return false;
}

bool GamepadController::isShare() const {
	if (m_isXBoxController) {
		return false;
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 8);
}

bool GamepadController::isOptions()const {
	if (m_isXBoxController) {
		return false;
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 9);
}

bool GamepadController::isLeftStickPush() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 8);
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 10);
}

bool GamepadController::isRightStickPush() const {
	if (m_isXBoxController) {
		return sf::Joystick::isButtonPressed(m_connectedJoystick, 9);
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 11);
}

bool GamepadController::isPSButton() const {
	if (m_isXBoxController) {
		return false;
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 12);
}

bool GamepadController::isTouchpad() const {
	if (m_isXBoxController) {
		return false;
	}
	return sf::Joystick::isButtonPressed(m_connectedJoystick, 13);
}

void GamepadController::initAxisMap() {
	m_axisMap.insert({ GamepadAxis::A, std::bind(&GamepadController::isA, this) });
	m_axisMap.insert({ GamepadAxis::B, std::bind(&GamepadController::isB, this) });
	m_axisMap.insert({ GamepadAxis::Y, std::bind(&GamepadController::isY, this) });
	m_axisMap.insert({ GamepadAxis::X, std::bind(&GamepadController::isX, this) });
	m_axisMap.insert({ GamepadAxis::Square, std::bind(&GamepadController::isSquare, this) });
	m_axisMap.insert({ GamepadAxis::Triangle, std::bind(&GamepadController::isTriangle, this) });
	m_axisMap.insert({ GamepadAxis::Circle, std::bind(&GamepadController::isCircle, this) });
	m_axisMap.insert({ GamepadAxis::DPadDown, std::bind(&GamepadController::isDPadDown, this) });
	m_axisMap.insert({ GamepadAxis::DPadLeft, std::bind(&GamepadController::isDPadLeft, this) });
	m_axisMap.insert({ GamepadAxis::DPadRight, std::bind(&GamepadController::isDPadRight, this) });
	m_axisMap.insert({ GamepadAxis::DPadUp, std::bind(&GamepadController::isDPadUp, this) });
	m_axisMap.insert({ GamepadAxis::LeftStickUp, std::bind(&GamepadController::isLeftStickUp, this) });
	m_axisMap.insert({ GamepadAxis::LeftStickDown, std::bind(&GamepadController::isLeftStickDown, this) });
	m_axisMap.insert({ GamepadAxis::LeftStickLeft, std::bind(&GamepadController::isLeftStickLeft, this) });
	m_axisMap.insert({ GamepadAxis::LeftStickRight, std::bind(&GamepadController::isLeftStickRight, this) });
	m_axisMap.insert({ GamepadAxis::RightStickUp, std::bind(&GamepadController::isRightStickUp, this) });
	m_axisMap.insert({ GamepadAxis::RightStickDown, std::bind(&GamepadController::isRightStickDown, this) });
	m_axisMap.insert({ GamepadAxis::RightStickLeft, std::bind(&GamepadController::isRightStickLeft, this) });
	m_axisMap.insert({ GamepadAxis::RightStickRight, std::bind(&GamepadController::isRightStickRight, this) });
	m_axisMap.insert({ GamepadAxis::LeftShoulder, std::bind(&GamepadController::isLeftShoulder, this) });
	m_axisMap.insert({ GamepadAxis::RightShoulder, std::bind(&GamepadController::isRightShoulder, this) });
	m_axisMap.insert({ GamepadAxis::LeftStickPush, std::bind(&GamepadController::isLeftStickPush, this) });
	m_axisMap.insert({ GamepadAxis::RightStickPush, std::bind(&GamepadController::isRightStickPush, this) });
	m_axisMap.insert({ GamepadAxis::LeftTrigger, std::bind(&GamepadController::isLeftTrigger, this) });
	m_axisMap.insert({ GamepadAxis::RightTrigger, std::bind(&GamepadController::isRightTrigger, this) });
	m_axisMap.insert({ GamepadAxis::Options, std::bind(&GamepadController::isOptions, this) });
	m_axisMap.insert({ GamepadAxis::PSButton, std::bind(&GamepadController::isPSButton, this) });
	m_axisMap.insert({ GamepadAxis::Select, std::bind(&GamepadController::isSelect, this) });
	m_axisMap.insert({ GamepadAxis::Start, std::bind(&GamepadController::isStart, this) });
	m_axisMap.insert({ GamepadAxis::Touchpad, std::bind(&GamepadController::isTouchpad, this) });
}

GamepadAxis GamepadController::getLastPressedGamepadAxisXbox(sf::Joystick::Axis axis, bool isNegative) {
	switch (axis)
	{
	case sf::Joystick::X:
		return isNegative ? GamepadAxis::LeftStickLeft : GamepadAxis::LeftStickRight;
	case sf::Joystick::Y:
		return isNegative ? GamepadAxis::LeftStickUp : GamepadAxis::LeftStickDown;
	case sf::Joystick::Z:
		return isNegative ? GamepadAxis::RightTrigger : GamepadAxis::LeftTrigger;
	case sf::Joystick::U:
		return isNegative ? GamepadAxis::RightStickLeft : GamepadAxis::RightStickRight;
	case sf::Joystick::V:
		return isNegative ? GamepadAxis::RightStickUp : GamepadAxis::RightStickDown;
	case sf::Joystick::PovX:
		return isNegative ? GamepadAxis::DPadLeft : GamepadAxis::DPadRight;
	case sf::Joystick::PovY:
		return isNegative ? GamepadAxis::DPadUp : GamepadAxis::DPadDown;
	default:
		return GamepadAxis::VOID;
	}
}

GamepadAxis GamepadController::getLastPressedGamepadAxisDs4(sf::Joystick::Axis axis, bool isNegative) {
	switch (axis)
	{
	case sf::Joystick::X:
		return isNegative ? GamepadAxis::LeftStickLeft : GamepadAxis::LeftStickRight;
	case sf::Joystick::Y:
		return isNegative ? GamepadAxis::LeftStickUp : GamepadAxis::LeftStickDown;
	case sf::Joystick::Z:
		return isNegative ? GamepadAxis::RightStickLeft : GamepadAxis::RightStickRight;
	case sf::Joystick::R:
		return isNegative ? GamepadAxis::RightStickUp : GamepadAxis::RightStickDown;
	case sf::Joystick::U:
		return GamepadAxis::RightTrigger;
	case sf::Joystick::V:
		return GamepadAxis::LeftTrigger;
	case sf::Joystick::PovX:
		return isNegative ? GamepadAxis::DPadLeft : GamepadAxis::DPadRight;
	case sf::Joystick::PovY:
		return isNegative ? GamepadAxis::DPadUp : GamepadAxis::DPadDown;
	default:
		return GamepadAxis::VOID;
	}
}

GamepadAxis GamepadController::getLastPressedGamepadButtonXbox(int button) {
	switch (button)
	{
	case 0: 
		return GamepadAxis::A;
	case 1:
		return GamepadAxis::B;
	case 2:
		return GamepadAxis::X;
	case 3:
		return GamepadAxis::Y;
	case 4:
		return GamepadAxis::LeftShoulder;
	case 5:
		return GamepadAxis::RightShoulder;
	case 6:
		return GamepadAxis::Select;
	case 7:
		return GamepadAxis::Start;
	case 8:
		return GamepadAxis::LeftStickPush;
	case 9:
		return GamepadAxis::RightStickPush;
	default:
		return GamepadAxis::VOID;
	}
}

GamepadAxis GamepadController::getLastPressedGamepadButtonDs4(int button) {
	switch (button)
	{
	case 0:
		return GamepadAxis::Square;
	case 1:
		return GamepadAxis::X;
	case 2:
		return GamepadAxis::Circle;
	case 3:
		return GamepadAxis::Triangle;
	case 4:
		return GamepadAxis::LeftShoulder;
	case 5:
		return GamepadAxis::RightShoulder;
	case 6:
		return GamepadAxis::LeftTrigger;
	case 7:
		return GamepadAxis::RightTrigger;
	case 8:
		return GamepadAxis::Share;
	case 9:
		return GamepadAxis::Options;
	case 10:
		return GamepadAxis::LeftStickPush;
	case 11:
		return GamepadAxis::RightStickPush;
	case 12:
		return GamepadAxis::PSButton;
	case 13:
		return GamepadAxis::Touchpad;
	default:
		return GamepadAxis::VOID;
	}
}
