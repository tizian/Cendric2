#include "GUI/SpellButtonGroup.h"
#include "GUI/ButtonInterface.h"
#include "GUI/SelectableWindow.h"
#include "Controller/InputController.h"

SpellButtonGroup::SpellButtonGroup() {
	m_selectedButtonIndexX = -1;
	m_selectedButtonIndexY = -1;
	m_isEnabled = true;
	m_isGamepadEnabled = true;
}

SpellButtonGroup::~SpellButtonGroup() {
}

void SpellButtonGroup::render(sf::RenderTarget& renderTarget) {
	for (auto& buttonRow : m_buttons) {
		for (auto button : buttonRow) {
			button->render(renderTarget);
		}
	}
}

void SpellButtonGroup::update(const sf::Time& frameTime) {
	if (!m_isEnabled) return;
	for (int y = 0; y < static_cast<int>(m_buttons.size()); ++y) {
		for (int x = 0; x < static_cast<int>(m_buttons[y].size()); ++x) {
			if (!m_buttons[y][x]->isVisibleAndEnabled()) {
				continue;
			}

			m_buttons[y][x]->update(frameTime);

			if (m_buttons[y][x]->isPressed()) {
				selectButton(y, x);
			}
		}
	}

	if (!m_isGamepadEnabled) return;
	updateSelection();
}

void SpellButtonGroup::setEnabled(bool enabled) {
	m_isEnabled = enabled;
	for (auto& buttonRow : m_buttons) {
		for (auto button : buttonRow) {
			button->setEnabled(enabled);
		}
	}
}

void SpellButtonGroup::setGamepadEnabled(bool enabled) {
	m_isGamepadEnabled = enabled;
}

void SpellButtonGroup::addButton(ButtonInterface* button, int yIndex) {
	if (yIndex < 0 || yIndex > static_cast<int>(m_buttons.size())) {
		return;
	}

	if (yIndex > static_cast<int>(m_buttons.size() - 1)) {
		m_buttons.push_back(std::vector<ButtonInterface*>());
	}

	m_buttons[yIndex].push_back(button);
	
	if (m_selectedButtonIndexX == -1) {
		selectButton(yIndex, 0);
		m_buttons[yIndex][0]->notifyFirstSelection();
	}
}

ButtonInterface* SpellButtonGroup::getSelectedButton() const {
	if (m_selectedButtonIndexX == -1) {
		return nullptr;
	}

	return m_buttons[m_selectedButtonIndexY][m_selectedButtonIndexX];
}

int SpellButtonGroup::getSelectedButtonIdX() const {
	return m_selectedButtonIndexX;
}

int SpellButtonGroup::getSelectedButtonIdY() const {
	return m_selectedButtonIndexY;
}

ButtonInterface* SpellButtonGroup::getButton(int yIndex, int xIndex) const {
	if (yIndex < 0 || yIndex > static_cast<int>(m_buttons.size()) - 1) {
		return nullptr;
	}

	if (xIndex < 0 || xIndex > static_cast<int>(m_buttons[yIndex].size()) - 1) {
		return nullptr;
	}

	return m_buttons[yIndex][xIndex];
}

void SpellButtonGroup::updateSelection() {
	if (g_inputController->isActionLocked() || m_selectedButtonIndexX == -1) {
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

	auto button = getSelectedButton();
	if (button->isVisibleAndEnabled() && g_inputController->isKeyJustPressed(Key::Confirm)) {
		button->click();
	}
}

void SpellButtonGroup::setNextButtonSelectedX(bool forward) {
	selectButton(m_selectedButtonIndexY, getNextEnabledButtonX(forward));
}

void SpellButtonGroup::setNextButtonSelectedY(bool forward) {
	selectButton(getNextEnabledButtonY(forward), m_selectedButtonIndexX);
}

int SpellButtonGroup::getNextEnabledButtonX(bool forward) {
	auto const& buttonRow = m_buttons[m_selectedButtonIndexY];

	if (forward) {
		if (m_selectedButtonIndexX + 1 == static_cast<int>(buttonRow.size())) {
			if (m_selectableWindow) {
				m_selectableWindow->setRightWindowSelected();
				g_inputController->lockAction();
			}
		}

		for (int i = m_selectedButtonIndexX + 1; i < static_cast<int>(buttonRow.size()); i++) {
			if (buttonRow[i]->isVisibleAndEnabled()) {
				return i;
			}
		}
	}
	else {
		if (m_selectedButtonIndexX == 0) {
			if (m_selectableWindow) {
				m_selectableWindow->setLeftWindowSelected();
				g_inputController->lockAction();
			}
		}

		for (int i = m_selectedButtonIndexX - 1; i > -1; i--) {
			if (buttonRow[i]->isVisibleAndEnabled()) {
				return i;
			}
		}
	}

	return m_selectedButtonIndexX;
}

int SpellButtonGroup::getNextEnabledButtonY(bool forward) {
	if (forward) {
		for (int i = m_selectedButtonIndexY + 1; i < static_cast<int>(m_buttons.size()); ++i) {
			if (static_cast<int>(m_buttons[i].size()) > m_selectedButtonIndexX) {
				return i;
			}

			if (static_cast<int>(m_buttons[i].size()) > 0) {
				return 1;
			}
		}
	}
	else {
		for (int i = m_selectedButtonIndexY - 1; i > -1; -i) {
			if (static_cast<int>(m_buttons[i].size()) > m_selectedButtonIndexX) {
				return i;
			}

			if (static_cast<int>(m_buttons[i].size()) > 0) {
				return 1;
			}
		}
	}

	return m_selectedButtonIndexY;
}

void SpellButtonGroup::selectButton(int yIndex, int xIndex) {
	if (yIndex < 0 || yIndex > static_cast<int>(m_buttons.size()) - 1) {
		return;
	}

	if (xIndex < 0 || xIndex > static_cast<int>(m_buttons[yIndex].size()) - 1) {
		return;
	}

	if (xIndex == m_selectedButtonIndexX && yIndex == m_selectedButtonIndexY) {
		return;
	}

	if (m_selectedButtonIndexX != -1) {
		getSelectedButton()->setSelected(false);
	}

	m_selectedButtonIndexY = yIndex;
	m_selectedButtonIndexX = xIndex;
	getSelectedButton()->setSelected(true);
	getSelectedButton()->notifySelection();
	g_inputController->lockAction();
}

void SpellButtonGroup::notifyButtonSelected(int yIndex, int xIndex) {
	if (yIndex < 0 || yIndex > static_cast<int>(m_buttons.size()) - 1) {
		return;
	}

	if (xIndex < 0 || xIndex > static_cast<int>(m_buttons[yIndex].size()) - 1) {
		return;
	}

	m_selectedButtonIndexY = yIndex;
	m_selectedButtonIndexX = xIndex;
}

void SpellButtonGroup::setSelectableWindow(SelectableWindow* window) {
	m_selectableWindow = window;
}

GameObjectType SpellButtonGroup::getConfiguredType() const {
	return GameObjectType::_Button;
}
