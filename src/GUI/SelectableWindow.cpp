#include "GUI/SelectableWindow.h"
#include "GUI/Window.h"

void SelectableWindow::setRightWindow(SelectableWindow* rightWindow) {
	m_rightWindow = rightWindow;
}

void SelectableWindow::setLeftWindow(SelectableWindow* leftWindow) {
	m_leftWindow = leftWindow;
}

void SelectableWindow::setWindowSelected(bool isSelected) {
	m_isWindowSelected = isSelected;
	updateWindowColor();
}

void SelectableWindow::updateWindowColor() {
	if (!m_window) {
		return;
	}

	m_window->setOrnamentColor(m_isWindowSelected ? COLOR_BRIGHT_PURPLE : COLOR_WHITE);
}

void SelectableWindow::setRightWindowSelected() {
	if (!m_rightWindow) {
		return;
	}

	m_isWindowSelected = false;
	m_rightWindow->setWindowSelected(true);
}

void SelectableWindow::setLeftWindowSelected() {
	if (!m_leftWindow) {
		return;
	}

	m_isWindowSelected = false;
	m_leftWindow->setWindowSelected(true);
}

bool SelectableWindow::isWindowSelected() const {
	return m_isWindowSelected;
}