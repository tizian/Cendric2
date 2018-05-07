#include "Controller/InputController.h"
#include "ResourceManager.h"

InputController* g_inputController;

InputController::InputController() {
	init();
}

InputController::~InputController() {
	m_keyboardKeyActiveMap.clear();
	m_keyboardKeyJustPressedMap.clear();
	m_gamepadKeyActiveMap.clear();
	m_gamepadKeyJustPressedMap.clear();
}

void InputController::update(const sf::Time& frameTime) {
	BaseController::update(frameTime);
	MouseController::update(frameTime);
	KeyboardController::update(frameTime);
	GamepadController::update(frameTime);

	// update keys
	for (auto& it : m_keyboardKeyActiveMap) {
		m_gamepadKeyJustPressedMap[it.first] = !m_gamepadKeyActiveMap[it.first] && isJoystickButtonPressed(it.first);
		m_gamepadKeyActiveMap[it.first] = isJoystickButtonPressed(it.first);
		m_keyboardKeyJustPressedMap[it.first] = !m_keyboardKeyActiveMap[it.first] && isKeyboardKeyPressed(it.first);
		m_keyboardKeyActiveMap[it.first] = isKeyboardKeyPressed(it.first);
	}
}

void InputController::init() {
	for (int i = static_cast<int>(Key::VOID) + 1; i < static_cast<int>(Key::MAX); ++i) {
		m_keyboardKeyActiveMap.insert({ static_cast<Key>(i), false });
	}

	m_keyboardKeyJustPressedMap = m_keyboardKeyActiveMap;
	m_gamepadKeyActiveMap = m_keyboardKeyActiveMap;
	m_gamepadKeyJustPressedMap = m_keyboardKeyActiveMap;
}

bool InputController::isKeyActive(Key key) const {
	return m_isWindowFocused && checkKeyActive(key);
}

bool InputController::isKeyJustPressed(Key key) const {
	if (m_isActionLocked || !m_isWindowFocused) return false;
	return checkKeyJustPressed(key);
}

bool InputController::isJoystickButtonJustPressed(Key key) const {
	if (m_isActionLocked || !m_isWindowFocused) return false;
	return m_gamepadKeyJustPressedMap.at(key);
}

bool InputController::checkKeyActive(Key key) const {
	return m_keyboardKeyActiveMap.at(key) || m_gamepadKeyActiveMap.at(key);
}

bool InputController::checkKeyJustPressed(Key key) const {
	return m_keyboardKeyJustPressedMap.at(key) || m_gamepadKeyJustPressedMap.at(key);
}

bool InputController::isSelected() const {
	return isKeyJustPressed(Key::Confirm) || isKeyJustPressed(Key::Jump) || isKeyJustPressed(Key::Interact);
}

bool InputController::isJustLeft() const {
	if (!m_isWindowFocused) return false;
	return isKeyJustPressed(Key::Left) || isLeftJoystickJustLeft();
}

bool InputController::isJustRight() const {
	if (!m_isWindowFocused) return false;
	return isKeyJustPressed(Key::Right) || isLeftJoystickJustRight();
}

bool InputController::isJustUp() const {
	if (!m_isWindowFocused) return false;
	return isKeyJustPressed(Key::Up) || isLeftJoystickJustUp();
}

bool InputController::isJustDown() const {
	if (!m_isWindowFocused) return false;
	return isKeyJustPressed(Key::Down) || isLeftJoystickJustDown();
}

bool InputController::isLeft() const {
	if (!m_isWindowFocused) return false;
	return checkKeyActive(Key::Left) || isLeftJoystickLeft();
}

bool InputController::isRight() const {
	if (!m_isWindowFocused) return false;
	return checkKeyActive(Key::Right) || isLeftJoystickRight();
}

bool InputController::isUp() const {
	if (!m_isWindowFocused) return false;
	return checkKeyActive(Key::Up) || isLeftJoystickUp();
}

bool InputController::isDown() const {
	if (!m_isWindowFocused) return false;
	return checkKeyActive(Key::Down) || isLeftJoystickDown();
}

bool InputController::isAttacking() const {
	return g_inputController->isMouseJustPressedLeft() || isKeyJustPressed(Key::Attack);
}

