#include "GUI/Slider.h"

using namespace std;

const float Slider::WIDTH = 100.f;
const float Slider::HEIGHT = 10.f;
const float Slider::MARGIN = 2.f;

Slider::Slider(int minPos, int maxPos) : GameObject() {
	if (minPos < 0) minPos = 0;
	if (maxPos < minPos) maxPos = minPos;
	m_minPosition = minPos;
	m_maxPosition = maxPos;
	setSliderPosition(m_minPosition);

	setSpriteOffset(sf::Vector2f(0.f, 0.f));

	m_background.setSize(sf::Vector2f(WIDTH, HEIGHT));
	m_background.setFillColor(m_backgroundColor);

	m_margin.setSize(sf::Vector2f(WIDTH, HEIGHT));
	m_margin.setFillColor(sf::Color::Transparent);
	m_margin.setOutlineThickness(-MARGIN);
	m_margin.setOutlineColor(m_marginColor);

	setBoundingBox(m_background.getGlobalBounds());
	setInputInDefaultView(true);
}

int Slider::getSliderPosition() const {
	return m_sliderPosition;
}

void Slider::onLeftClick() {
	if (m_isEnabled && m_isPressed) {
		m_isPressed = false;
		g_inputController->lockAction();
	}
}

void Slider::setSliderPosition(int pos) {
	if (pos < m_minPosition) pos = m_minPosition;
	if (pos > m_maxPosition) pos = m_maxPosition;

	m_sliderPosition = pos;
}

void Slider::onLeftJustPressed() {
	if (m_isEnabled) {
		m_isPressed = true;
		g_inputController->lockAction();
	}
}

void Slider::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	renderTarget.draw(m_background);
	renderTarget.draw(m_margin);
	m_knob.render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_minText);
	renderTarget.draw(m_maxText);
}

void Slider::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_background.setPosition(pos);
	m_margin.setPosition(pos);
}

void Slider::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	GameObject::update(frameTime);
}

void Slider::setKnobColor(const sf::Color& color) {
	m_knob.setColor(color);
}

void Slider::setBackgroundColor(const sf::Color& color) {
	m_background.setFillColor(color);
	m_backgroundColor = color;
}

void Slider::setMarginColor(const sf::Color& color) {
	m_margin.setOutlineColor(color);
	m_marginColor = color;
}

void Slider::setTextColor(const sf::Color& color) {
	m_titleText.setColor(color);
	m_maxText.setColor(color);
	m_minText.setColor(color);
}

void Slider::setText(const std::string& text, const sf::Color& color, int charSize) {
	m_titleText = BitmapText(g_textProvider->getText(text));

	m_titleText.setColor(color);
	setCharacterSize(charSize);
}

void Slider::setText(const std::string& text) {
	setText(text, sf::Color::White, 12);
}

void Slider::setText(const std::string& text, int charSize) {
	setText(text, sf::Color::White, charSize);
}

void Slider::setTextRaw(const std::string& text, const sf::Color& color, int charSize) {
	m_titleText = BitmapText(text);

	m_titleText.setColor(color);
	setCharacterSize(charSize);
}

void Slider::setTextRaw(const std::string& text, int charSize) {
	setTextRaw(text, sf::Color::White, charSize);
}

void Slider::setTextRaw(const std::string& text) {
	setTextRaw(text, sf::Color::White, 12);
}

void Slider::setCharacterSize(int size) {
	m_titleText.setCharacterSize(size);
	float xOffset = WIDTH / 2.f - m_titleText.getLocalBounds().width / 2.f;
	float yOffset = -(10.f + size);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
	m_titleText.setPosition(getPosition() + m_textOffset);
}

void Slider::setEnabled(bool enabled) {
	m_isEnabled = enabled;
	m_margin.setOutlineColor(sf::Color(
		m_margin.getOutlineColor().r,
		m_margin.getOutlineColor().g,
		m_margin.getOutlineColor().b,
		m_isEnabled ? 255 : 100));
	m_titleText.setColor(sf::Color(
		m_titleText.getColor().r,
		m_titleText.getColor().g,
		m_titleText.getColor().b,
		m_isEnabled ? 255 : 100));
}

void Slider::setVisible(bool value) {
	m_isVisible = value;
}

bool Slider::isEnabled() const {
	return m_isEnabled;
}

bool Slider::isVisible() const {
	return m_isVisible;
}

GameObjectType Slider::getConfiguredType() const {
	return GameObjectType::_Button;
}

// SLIDER KNOB

SliderKnob::SliderKnob() : GameObject() {

}

void SliderKnob::onLeftJustPressed() {

}

void SliderKnob::onLeftClick() {

}

void SliderKnob::onMouseOver() {

}

void SliderKnob::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_knob);
}

void SliderKnob::update(const sf::Time& frameTime) {
	GameObject::update(frameTime);
}

void SliderKnob::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
}

void SliderKnob::setColor(const sf::Color& color) {
	m_knob.setFillColor(color);
}

GameObjectType SliderKnob::getConfiguredType() const {
	return GameObjectType::_Button;
}