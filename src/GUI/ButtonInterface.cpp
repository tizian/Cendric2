#include "GUI/ButtonInterface.h"

void ButtonInterface::setEnabled(bool enabled) {
	m_isEnabled = enabled;
}

void ButtonInterface::setSelected(bool selected) {
	m_isSelected = selected;
	updateColor();
}

void ButtonInterface::setVisible(bool visible) {
	m_isVisible = visible;
}

bool ButtonInterface::isClicked() const {
	return m_isClicked;
}

bool ButtonInterface::isPressed() const {
	return m_isPressed;
}

bool ButtonInterface::isEnabled() const {
	return m_isEnabled;
}

bool ButtonInterface::isVisible() const {
	return m_isVisible;
}

bool ButtonInterface::isSelected() const {
	return m_isSelected;
}

bool ButtonInterface::isVisibleAndEnabled() const {
	return m_isVisible && m_isEnabled;
}

GameObjectType ButtonInterface::getConfiguredType() const {
	return _Button;
}
