#include "GUI/ArrowButton.h"

ArrowButton::ArrowButton(bool pointRight) : GameObject() {
	m_arrow.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_GUI_arrow));

	// center origin
	m_arrow.setOrigin(sf::Vector2f(m_arrow.getLocalBounds().width / 2.f, m_arrow.getLocalBounds().height / 2.f));
	m_arrow.rotate(pointRight ? 90.f : -90.f);
	setBoundingBox(sf::FloatRect(0.f, 0.f, m_arrow.getLocalBounds().height, m_arrow.getLocalBounds().width));

	// default colors 
	m_mainColor = COLOR_WHITE;
	m_disabledColor = COLOR_DARK_GREY;
	m_mouseoverColor = sf::Color(91, 73, 133);

	m_isInputInDefaultView = true;
}

void ArrowButton::onLeftClick() {
	if (!m_isEnabled) return;
	m_isClicked = true;
	g_inputController->lockAction();
}

bool ArrowButton::isClicked() const {
	return m_isClicked;
}

void ArrowButton::onMouseOver() {
	if (!m_isEnabled) return;
	m_isMouseOver = true;
}

void ArrowButton::setEnabled(bool value) {
	m_isEnabled = value;
	m_arrow.setColor(m_isEnabled ? m_mainColor : m_disabledColor);
}

void ArrowButton::setMouseoverColor(const sf::Color& color) {
	m_mouseoverColor = color;
	recalculateColor();
}

void ArrowButton::setMainColor(const sf::Color& color) {
	m_mainColor = color;
	recalculateColor();
}

void ArrowButton::setDisabledColor(const sf::Color& color) {
	m_disabledColor = color;
	recalculateColor();
}

void ArrowButton::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_arrow);
}

void ArrowButton::update(const sf::Time& frameTime) {
	m_isClicked = false;
	if (m_isMouseOver != m_wasMouseOver) {
		recalculateColor();
	}
	m_wasMouseOver = m_isMouseOver;
	m_isMouseOver = false;
	GameObject::update(frameTime);
}

void ArrowButton::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_arrow.setPosition(pos + 0.5f * getSize());
}

GameObjectType ArrowButton::getConfiguredType() const {
	return GameObjectType::_Button;
}

void ArrowButton::recalculateColor() {
	m_arrow.setColor(m_isMouseOver ? m_mouseoverColor : m_isEnabled ? m_mainColor : m_disabledColor);
}