#include "Controller/GamepadController.h"
#include "ResourceManager.h"

const int GamepadController::AXIS_THRESHOLD = 50;

GamepadController::GamepadController() {
	m_joystickMap = &(g_resourceManager->getConfiguration().joystickKeyMap);
}

void GamepadController::update(const sf::Time& frameTime) {
	if (!isJoystickConnected()) return;
	updateLeftJoystick();
}

bool GamepadController::isJoystickConnected() const {
	return m_connectedJoystick > -1;
}

void GamepadController::updateLeftJoystick() {
	bool joystickLeft = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::X) < -AXIS_THRESHOLD;
	bool dpadLeft = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovX) < -AXIS_THRESHOLD;
	bool left = joystickLeft || dpadLeft;
	m_isLeftJoystickLeftJustPressed = !m_isLeftJoystickLeftPressed && left;
	m_isLeftJoystickLeftPressed = left;

	bool joystickRight = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::X) > AXIS_THRESHOLD;
	bool dpadRight = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovX) > AXIS_THRESHOLD;
	bool right = joystickRight || dpadRight;
	m_isLeftJoystickRightJustPressed = !m_isLeftJoystickRightPressed && right;
	m_isLeftJoystickRightPressed = right;

	bool joystickUp = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Y) < -AXIS_THRESHOLD;
	bool dpadUp = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovY) > AXIS_THRESHOLD;
	bool up = joystickUp || dpadUp;
	m_isLeftJoystickUpJustPressed = !m_isLeftJoystickUpPressed && up;
	m_isLeftJoystickUpPressed = up;

	bool joystickDown = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Y) > AXIS_THRESHOLD;
	bool dpadDown = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::PovY) < -AXIS_THRESHOLD;
	bool down = joystickDown || dpadDown;
	m_isLeftJoystickDownJustPressed = !m_isLeftJoystickDownPressed && down;
	m_isLeftJoystickDownPressed = down;
}

bool GamepadController::isJoystickButtonPressed(Key key) const {
	auto const it = m_joystickMap->find(key);
	if (it == m_joystickMap->end()) {
		return false;
	}

	return isJoystickButtonPressed((*it).second);
}

bool GamepadController::isJoystickButtonPressed(int button) const {
	if (button < 0) {
		return false;
	}
	return sf::Joystick::isButtonPressed(0, button);
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
		axis.y = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::R);
	}
	else {
		axis.x = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::Z);
		axis.y = sf::Joystick::getAxisPosition(m_connectedJoystick, sf::Joystick::R);
	}
	
	return axis;
}

void GamepadController::notifyJoystickConnected() {
	m_connectedJoystick = -1;
	for (int i = 0; i < 8; i++) {
		if (sf::Joystick::isConnected(i)) {
			m_connectedJoystick = i;
			std::string name = sf::Joystick::getIdentification(i).name;
			m_isXBoxController = name.find("Xbox") != std::string::npos;
			break;
		}
	}
}

