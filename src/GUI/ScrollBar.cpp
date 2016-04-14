#include "GUI/ScrollBar.h"

using namespace std;

const float ScrollBar::WIDTH = 20.f;

const sf::Color ScrollBar::BACKGROUND_COLOR = COLOR_TRANS_BLACK;
const sf::Color ScrollBar::FILL_COLOR = COLOR_WHITE;

ScrollBar::ScrollBar(float height) : GameObject() {
	m_discreteSteps = 1;

	m_height = height - ScrollBarKnob::HEIGHT;
	setScrollPosition(0.f);

	m_background.setSize(sf::Vector2f(WIDTH - 4.f, height - 4.f));
	m_background.setFillColor(BACKGROUND_COLOR);

	m_border = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_window_border), COLOR_WHITE, WIDTH, height);

	setBoundingBox(m_background.getGlobalBounds());
	setInputInDefaultView(true);
}

float ScrollBar::getScrollPosition() const {
	return m_scrollPosition;
}

void ScrollBar::onLeftClick() {
	if (m_isEnabled && m_isPressed) {
		m_isPressed = false;
		g_inputController->lockAction();

		// update the position of the slider if it was clicked.
		sf::Vector2f mousePos = g_inputController->getDefaultViewMousePosition();
		int direction;
		if (mousePos.y > m_knob.getPosition().y) {
			direction = 1;
		}
		else {
			direction = -1;
		}

		scroll(direction);
	}
}

void ScrollBar::setDiscreteSteps(int steps) {
	m_discreteSteps = steps;
}

void ScrollBar::scroll(int direction) {
	if (m_discreteSteps >= 2) {
		float delta = 1.f / (m_discreteSteps - 1);
		setScrollPosition(m_scrollPosition + direction * delta);
	}
	else {
		setScrollPosition(m_scrollPosition + direction * 0.1f);
	}
}

void ScrollBar::setScrollPosition(float pos) {
	if (pos < 0.f) pos = 0.f;
	if (pos > 1.f) pos = 1.f;
	
	float snappedPosition = pos;

	if (m_discreteSteps >= 2) {
		float delta = 1.f / (m_discreteSteps - 1);
		int floored = static_cast<int>(std::floor(pos / delta));
		snappedPosition = floored * delta;
		if (std::fmod(pos, delta) >= 0.5f * delta) {
			snappedPosition += delta;
		}
	}

	m_scrollPosition = snappedPosition;

	// update knob
	float yPos = 0.5f * ScrollBarKnob::HEIGHT + m_scrollPosition * m_height;
	m_knob.setPosition(sf::Vector2f(getBoundingBox()->left + 0.5f * WIDTH, getBoundingBox()->top + yPos));
}

void ScrollBar::onLeftJustPressed() {
	if (m_isEnabled) {
		m_isPressed = true;
		g_inputController->lockAction();
	}
}

void ScrollBar::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	renderTarget.draw(m_background);
	renderTarget.draw(m_border);

	if (m_isEnabled) {
		m_knob.render(renderTarget);
	}
}

void ScrollBar::handleDragAndDrop() {
	sf::Vector2f mousePos = g_inputController->getDefaultViewMousePosition();
	setScrollPosition(calculateScrollPosition(mousePos.y));
}

float ScrollBar::calculateScrollPosition(float mousePosY) const {
	float yOffset = mousePosY - getBoundingBox()->top - 0.5f * ScrollBarKnob::HEIGHT;
	// map the offset onto the values
	return yOffset / m_height;
}

void ScrollBar::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_background.setPosition(sf::Vector2f(pos.x + 2.f, pos.y + 2.f));
	m_border.setPosition(pos);
	setScrollPosition(m_scrollPosition);
}

void ScrollBar::update(const sf::Time& frameTime) {
	if (!m_isVisible || !m_isEnabled) return;

	if (g_inputController->isMouseWheelScrolledUp()) {
		scroll(-1);
	}
	else if (g_inputController->isMouseWheelScrolledDown()) {
		scroll(1);
	}

	m_knob.update(frameTime);
	if (m_knob.isPressed()) {
		handleDragAndDrop();
	}
	GameObject::update(frameTime);
}

void ScrollBar::setEnabled(bool enabled) {
	m_isEnabled = enabled;
}

void ScrollBar::setVisible(bool value) {
	m_isVisible = value;
}

bool ScrollBar::isEnabled() const {
	return m_isEnabled;
}

bool ScrollBar::isVisible() const {
	return m_isVisible;
}

GameObjectType ScrollBar::getConfiguredType() const {
	return GameObjectType::_Button;
}

// SLIDER KNOB

const float ScrollBarKnob::WIDTH = 30.f;
const float ScrollBarKnob::HEIGHT = 20.f;

ScrollBarKnob::ScrollBarKnob() : GameObject() {
	m_knob.setSize(sf::Vector2f(WIDTH, HEIGHT));
	m_knob.setTexture(g_resourceManager->getTexture(ResourceID::Texture_GUI_window_border));

	setBoundingBox(m_knob.getLocalBounds());
	setInputInDefaultView(true);
}

void ScrollBarKnob::onLeftJustPressed() {
	m_isPressed = true;
	g_inputController->lockAction();
}

void ScrollBarKnob::onLeftClick() {
	m_isPressed = false;
	g_inputController->lockAction();
}

bool ScrollBarKnob::isPressed() const {
	return m_isPressed;
}

void ScrollBarKnob::onMouseOver() {
}

void ScrollBarKnob::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_knob);
}

void ScrollBarKnob::update(const sf::Time& frameTime) {
	GameObject::update(frameTime);
	if (m_isPressed && !g_inputController->isMousePressedLeft()) {
		m_isPressed = false;
	}
}

void ScrollBarKnob::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos - sf::Vector2f(0.5f * WIDTH, 0.5f * HEIGHT));
	m_knob.setPosition(pos - sf::Vector2f(0.5f * WIDTH, 0.5f * HEIGHT));
}

GameObjectType ScrollBarKnob::getConfiguredType() const {
	return GameObjectType::_Button;
}