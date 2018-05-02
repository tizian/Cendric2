#include "GUI/ButtonGroup.h"
#include "GUI/Button.h"
#include "Controller/InputController.h"

ButtonGroup::ButtonGroup() {
	m_selectedButtonIndex = -1;
	m_isEnabled = true;
	m_isHorizontal = false;
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
	for (auto button : m_buttons) {
		button->update(frameTime);
	}

	updateSelection();
}

void ButtonGroup::setEnabled(bool enabled) {
	m_isEnabled = enabled;
	for (auto button : m_buttons) {
		button->setEnabled(enabled);
	}
}

void ButtonGroup::setHorizontal(bool horizontal) {
	m_isHorizontal = horizontal;
}

void ButtonGroup::addButton(Button* button) {
	m_buttons.push_back(button);
	if (m_selectedButtonIndex == -1) {
		selectButton(0);
	}
}

Button* ButtonGroup::getSelectedButton() const {
	return m_buttons[m_selectedButtonIndex];
}

void ButtonGroup::updateSelection() {
	if (g_inputController->isActionLocked() || m_selectedButtonIndex == -1) {
		return;
	}

	if (m_isHorizontal) {
		if (g_inputController->isJustLeft()) {
			setNextButtonSelected(false);
		}
		else if (g_inputController->isJustRight()) {
			setNextButtonSelected(true);
		}
	}
	else {
		if (g_inputController->isJustUp()) {
			setNextButtonSelected(false);
		}
		else if (g_inputController->isJustDown()) {
			setNextButtonSelected(true);
		}
	}

	if (g_inputController->isSelected()) {
		m_buttons[m_selectedButtonIndex]->click();
	}
}

void ButtonGroup::setNextButtonSelected(bool forward) {
	selectButton(getNextEnabledButton(forward));
}

int ButtonGroup::getNextEnabledButton(bool forward) {
	if (forward) {
		for (int i = m_selectedButtonIndex + 1; i < static_cast<int>(m_buttons.size()); i++) {
			if (m_buttons[i]->isEnabled()) {
				return i;
			}
		}
	}
	else {
		for (int i = m_selectedButtonIndex - 1; i > -1; i--) {
			if (m_buttons[i]->isEnabled()) {
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

GameObjectType ButtonGroup::getConfiguredType() const {
	return GameObjectType::_Button;
}
