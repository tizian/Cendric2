#include "GUI/SelectableWindow.h"
#include "GUI/Window.h"

void SelectableWindow::setRightWindow(SelectableWindow* rightWindow) {
	if (m_rightWindow) {
		m_rightWindow->m_leftWindow = nullptr;
	}
	
	m_rightWindow = rightWindow;

	if (m_rightWindow) {
		m_rightWindow->m_leftWindow = this;
	}
}

void SelectableWindow::setLeftWindow(SelectableWindow* leftWindow) {
	if (m_leftWindow) {
		m_leftWindow->m_rightWindow = nullptr;
	}
	
	m_leftWindow = leftWindow;

	if (m_leftWindow) {
		m_leftWindow->m_rightWindow = this;
	}
}

void SelectableWindow::setWindowSelected(bool isSelected) {
	m_isWindowSelected = isSelected;
	updateWindowColor();
	updateWindowSelected();
}

void SelectableWindow::updateWindowColor() const {
	if (!m_window) {
		return;
	}

	m_window->setOrnamentColor(isWindowSelected() ? COLOR_BRIGHT_PURPLE : COLOR_WHITE);
}

void SelectableWindow::setRightWindowSelected() {
	if (!m_rightWindow) {
		return;
	}

	setWindowSelected(false);
	m_rightWindow->setWindowSelected(true);
}

void SelectableWindow::setLeftWindowSelected() {
	if (!m_leftWindow) {
		return;
	}

	setWindowSelected(false);
	m_leftWindow->setWindowSelected(true);
}

bool SelectableWindow::isWindowSelected() const {
	return m_isWindowSelected && g_inputController->isGamepadConnected();
}