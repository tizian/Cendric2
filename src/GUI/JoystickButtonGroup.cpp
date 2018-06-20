#include "GUI/JoystickButtonGroup.h"
#include "GUI/ButtonInterface.h"
#include "GUI/SelectableWindow.h"
#include "Controller/InputController.h"

#include <limits.h>

JoystickButtonGroup::JoystickButtonGroup() {
	m_selectedButtonIndex = -1;
	m_isGamepadEnabled = true;
	m_isJoystickDown = false;
}

void JoystickButtonGroup::update(const sf::Time& frameTime) {
	if (!m_isGamepadEnabled) return;
	updateSelection();

	if (m_selectedButtonIndex != -1) {
		auto button = m_buttons[m_selectedButtonIndex];
		if (g_inputController->isKeyJustPressed(Key::Confirm)) {
			button->click();
		}
	}
}

void JoystickButtonGroup::setGamepadEnabled(bool enabled) {
	m_isGamepadEnabled = enabled;
}

void JoystickButtonGroup::addButton(ButtonInterface* button) {
	m_buttons.push_back(button);
	int index = static_cast<int>(m_buttons.size()) - 1;
	if (m_selectedButtonIndex == -1 && m_buttons[index]->isVisibleAndEnabled()) {
		selectButton(index);
		m_buttons[index]->notifyFirstSelection();
	}
}

void JoystickButtonGroup::updateSelection() {
	if (g_inputController->isActionLocked()) {
		return;
	}

	if (m_selectedButtonIndex == -1) {
		if (g_inputController->isJustLeft()) {
			if (m_selectableWindow) {
				m_selectableWindow->setLeftWindowSelected();
				g_inputController->lockAction();
			}
		}
		else if (g_inputController->isJustRight()) {
			if (m_selectableWindow) {
				m_selectableWindow->setRightWindowSelected();
				g_inputController->lockAction();
			}
		}
		return;
	}

	const auto axis = g_inputController->getAnyMoveJoystickAxis();
	if (norm(axis) < GamepadController::AXIS_THRESHOLD) {
		m_isJoystickDown = false;
		return;
	}

	if (m_isJoystickDown) {
		return;
	}

	g_inputController->lockAction();
	m_isJoystickDown = true;

	const auto currentPosition = m_buttons[m_selectedButtonIndex]->getCenter();
	const auto axisAngle = std::atan2(axis.y, axis.x);

	auto bestDistance = static_cast<float>(INT_MAX);
	auto bestButtonIndex = -1;

	for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i) {
		if (i == m_selectedButtonIndex) continue;

		const auto otherPosition = m_buttons[i]->getCenter();
		const auto distVector = otherPosition - currentPosition;
		const auto distance = norm(distVector);

		if (distance < Epsilon) continue;

		const auto distAngle = std::atan2(distVector.y, distVector.x);

		if (std::abs(distAngle - axisAngle) > M_PI * 0.25f) continue;

		if (distance < bestDistance) {
			bestDistance = distance;
			bestButtonIndex = i;
		}
	}

	if (bestButtonIndex != -1) {
		selectButton(bestButtonIndex);
	}
	else if (m_selectableWindow && std::abs(axis.x) > std::abs(axis.y)) {
		if (axis.x > 0) {
			m_selectableWindow->setRightWindowSelected();
		}
		else {
			m_selectableWindow->setLeftWindowSelected();
		}
	}
}

const std::vector<ButtonInterface*>& JoystickButtonGroup::getButtons() const {
	return m_buttons;
}

void JoystickButtonGroup::selectButton(int index) {
	if (index < 0 || index > static_cast<int>(m_buttons.size()) - 1) {
		return;
	}

	if (index == m_selectedButtonIndex) {
		return;
	}

	if (m_selectedButtonIndex != -1) {
		m_buttons[m_selectedButtonIndex]->setSelected(false);
	}

	m_selectedButtonIndex = index;
	m_buttons[index]->setSelected(true);
	g_inputController->lockAction();
}

void JoystickButtonGroup::setSelectableWindow(SelectableWindow* window) {
	m_selectableWindow = window;
}

void JoystickButtonGroup::notifyJoystickDown() {
	m_isJoystickDown = true;
}

GameObjectType JoystickButtonGroup::getConfiguredType() const {
	return GameObjectType::_Button;
}
