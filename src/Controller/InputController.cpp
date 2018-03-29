#include "Controller/InputController.h"
#include "ResourceManager.h"

InputController* g_inputController;

InputController::InputController() {
	init();
}

InputController::~InputController() {
	m_keyActiveMap.clear();
	m_keyJustPressedMap.clear();
}

void InputController::update(const sf::Time& frameTime) {
	BaseController::update(frameTime);
	MouseController::update(frameTime);
	KeyboardController::update(frameTime);
	GamepadController::update(frameTime);

	// update keys
	for (auto& it : m_keyActiveMap) {
		m_keyJustPressedMap[it.first] = !m_keyActiveMap[it.first] && checkKeyActive(it.first);
		m_keyActiveMap[it.first] = checkKeyActive(it.first);
	}
}

void InputController::init() {
	for (int i = static_cast<int>(Key::VOID) + 1; i < static_cast<int>(Key::MAX); ++i) {
		m_keyActiveMap.insert({ static_cast<Key>(i), false });
	}

	m_keyJustPressedMap = m_keyActiveMap;
}

bool InputController::isKeyActive(Key key) const {
	return m_isWindowFocused && m_keyActiveMap.at(key);
}

bool InputController::isKeyJustPressed(Key key) const {
	if (m_isActionLocked || !m_isWindowFocused) return false;
	return m_keyJustPressedMap.at(key);
}

bool InputController::checkKeyActive(Key key) const {
    return isKeyboardKeyPressed(key) || isJoystickButtonPressed(key);
}

bool InputController::isSelected() const {
	return isKeyJustPressed(Key::Confirm) || isKeyJustPressed(Key::Jump) || isKeyJustPressed(Key::Interact);
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

