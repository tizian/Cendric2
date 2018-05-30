#include "GUI/ButtonGroup.h"
#include "GUI/ButtonInterface.h"
#include "GUI/SelectableWindow.h"
#include "Controller/InputController.h"

ButtonGroup::ButtonGroup(int width) {
	m_selectedButtonIndex = -1;
	m_width = width;
	m_isEnabled = true;
	m_isGamepadEnabled = true;
}

ButtonGroup::~ButtonGroup() {
	CLEAR_VECTOR(m_buttons);
}

void ButtonGroup::render(sf::RenderTarget& renderTarget) {
	for (auto button : m_buttons) {
		button->render(renderTarget);
	}
}

void ButtonGroup::update(const sf::Time& frameTime) {
	if (!m_isEnabled) return;
	for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i) {
		if (!m_buttons[i]->isVisibleAndEnabled()) {
			continue;
		}

		m_buttons[i]->update(frameTime);

		if (m_buttons[i]->isPressed()) {
			selectButton(i);
		}
	}

	if (!m_isGamepadEnabled) return;
	updateSelection();
}

void ButtonGroup::setEnabled(bool enabled) {
	m_isEnabled = enabled;
	for (auto button : m_buttons) {
		button->setEnabled(enabled);
	}
}

void ButtonGroup::setGamepadEnabled(bool enabled) {
	m_isGamepadEnabled = enabled;
}

void ButtonGroup::addButton(ButtonInterface* button) {
	m_buttons.push_back(button);
	if (m_selectedButtonIndex == -1) {
		selectButton(0);
		m_buttons[0]->notifyFirstSelection();
	}
}

ButtonInterface* ButtonGroup::getSelectedButton() const {
	return m_buttons[m_selectedButtonIndex];
}

ButtonInterface* ButtonGroup::getButton(int i) const {
	if (i < 0 || i > static_cast<int>(m_buttons.size()) - 1) {
		return nullptr;
	}

	return m_buttons[i];
}

const std::vector<ButtonInterface*>& ButtonGroup::getButtons() const {
	return m_buttons;
}

void ButtonGroup::updateSelection() {
	if (g_inputController->isActionLocked() || m_selectedButtonIndex == -1) {
		return;
	}

	if (g_inputController->isJustLeft()) {
		setNextButtonSelectedX(false);
	}
	else if (g_inputController->isJustRight()) {
		setNextButtonSelectedX(true);
	}
	else if (g_inputController->isJustUp()) {
		setNextButtonSelectedY(false);
	}
	else if (g_inputController->isJustDown()) {
		setNextButtonSelectedY(true);
	}

	auto button = m_buttons[m_selectedButtonIndex];
	if (button->isVisibleAndEnabled() && g_inputController->isKeyJustPressed(Key::Confirm)) {
		button->click();
	}
}

void ButtonGroup::setNextButtonSelectedX(bool forward) {
	selectButton(getNextEnabledButtonX(forward));
}

void ButtonGroup::setNextButtonSelectedY(bool forward) {
	selectButton(getNextEnabledButtonY(forward));
}

int ButtonGroup::getNextEnabledButtonX(bool forward) {
	int x = getSelectedIndexX();
	int y = getSelectedIndexY();

	if (forward) {
		if (x + 1 == m_width) {
			if (m_selectableWindow) {
				m_selectableWindow->setRightWindowSelected();
				g_inputController->lockAction();
			}
		}

		for (int i = y * m_width + x + 1; i < y * m_width + m_width; i++) {
			if (m_buttons[i]->isVisibleAndEnabled()) {
				return i;
			}
		}
	}
	else {
		if (x == 0) {
			if (m_selectableWindow) {
				m_selectableWindow->setLeftWindowSelected();
				g_inputController->lockAction();
			}
		}

		for (int i = y * m_width + x - 1; i > y * m_width - 1; i--) {
			if (m_buttons[i]->isVisibleAndEnabled()) {
				return i;
			}
		}
	}

	return m_selectedButtonIndex;
}

int ButtonGroup::getNextEnabledButtonY(bool forward) {
	int y = getSelectedIndexY();

	if (forward) {
		for (int i = (y + 1) * m_width; i < static_cast<int>(m_buttons.size()); i += m_width) {
			if (m_buttons[i]->isVisibleAndEnabled()) {
				return i;
			}
		}
	}
	else {
		for (int i = (y - 1) * m_width; i > -1; i -= m_width) {
			if (m_buttons[i]->isVisibleAndEnabled()) {
				return i;
			}
		}
	}

	return m_selectedButtonIndex;
}

void ButtonGroup::selectButton(int index) {
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

void ButtonGroup::setSelectableWindow(SelectableWindow* window) {
	m_selectableWindow = window;
}

int ButtonGroup::getSelectedIndexX() const {
	if (m_selectedButtonIndex < 0 || m_width < 1) {
		return -1;
	}

	return m_selectedButtonIndex % m_width;
}

int ButtonGroup::getSelectedIndexY() const {
	if (m_selectedButtonIndex < 0 || m_width < 1) {
		return -1;
	}

	return m_selectedButtonIndex / m_width;
}

GameObjectType ButtonGroup::getConfiguredType() const {
	return GameObjectType::_Button;
}
