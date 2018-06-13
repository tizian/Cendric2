#include "GUI/Slider.h"
#include "GlobalResource.h"

const float Slider::WIDTH = 300.f;
const float Slider::HEIGHT = 20.f;
const float Slider::BORDER_OFFSET = 7.f;

Slider::Slider(int minPos, int maxPos) {
	if (minPos < 0) minPos = 0;
	if (maxPos < minPos) maxPos = minPos;
	m_minPosition = minPos;
	m_maxPosition = maxPos;
	setSliderPosition(m_minPosition);

	m_background.setSize(sf::Vector2f(WIDTH, HEIGHT));
	m_background.setFillColor(COLOR_BLACK);

	m_filler.setSize(sf::Vector2f(WIDTH, HEIGHT));

	m_border = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_SLIDER), COLOR_WHITE, 2.f * BORDER_OFFSET + WIDTH, 2.f * BORDER_OFFSET + HEIGHT);

	m_minText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_minText.setString(std::to_string(m_minPosition));
	m_maxText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_maxText.setString(std::to_string(m_maxPosition));

	setCharacterSize(GUIConstants::CHARACTER_SIZE_M);

	setBoundingBox(m_background.getGlobalBounds());
	setInputInDefaultView(true);

	m_unit = "";

	updateColor();
}

int Slider::getSliderPosition() const {
	return m_sliderPosition;
}

void Slider::onLeftClick() {
	if (m_isEnabled && m_isPressed) {
		m_isPressed = false;
		g_inputController->lockAction();

		// update the position of the slider if it was clicked.
		sf::Vector2f mousePos = g_inputController->getDefaultViewMousePosition();
		int length = m_maxPosition - m_minPosition;
		if (mousePos.x > m_knob.getPosition().x) {
			setSliderPosition(m_sliderPosition + static_cast<int>(length / 10.f));
		}
		else {
			setSliderPosition(m_sliderPosition - static_cast<int>(length / 10.f));
		}
	}
}

void Slider::setSliderPosition(int pos) {
	if (pos < m_minPosition) pos = m_minPosition;
	if (pos > m_maxPosition) pos = m_maxPosition;

	m_sliderPosition = pos;

	// update knob
	int length = m_maxPosition - m_minPosition;
	float xPos = (float)(m_sliderPosition - m_minPosition) / (float)length * WIDTH;
	m_knob.setPosition(sf::Vector2f(getBoundingBox()->left + xPos, getBoundingBox()->top + HEIGHT / 2.f));

	// update filler
	m_filler.setSize(sf::Vector2f(xPos, HEIGHT));

	// update text
	setTextRaw(m_title);
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
	renderTarget.draw(m_filler);
	renderTarget.draw(m_border);
	m_knob.render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_minText);
	renderTarget.draw(m_maxText);
}

void Slider::handleDragAndDrop() {
	sf::Vector2f mousePos = g_inputController->getDefaultViewMousePosition();
	setSliderPosition(calculateSliderPosition(mousePos.x));
}

void Slider::updateColor() {
	m_filler.setFillColor(m_isSelected ? COLOR_MEDIUM_PURPLE : COLOR_PURPLE);
	m_titleText.setColor(m_isSelected ? COLOR_BRIGHT_PURPLE : COLOR_WHITE);
}

int Slider::calculateSliderPosition(float mousePosX) const {
	float xOffset = mousePosX - getBoundingBox()->left;
	// map the offset onto the values
	int length = m_maxPosition - m_minPosition;
	return static_cast<int>(length * xOffset / WIDTH) + m_minPosition;
}

void Slider::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_background.setPosition(pos);
	m_filler.setPosition(pos);
	m_border.setPosition(sf::Vector2f(pos.x - BORDER_OFFSET, pos.y - BORDER_OFFSET));
	m_minText.setPosition(sf::Vector2f(
		pos.x,
		pos.y + HEIGHT + 10.f));
	m_maxText.setPosition(sf::Vector2f(
		pos.x + WIDTH - m_maxText.getLocalBounds().width,
		pos.y + HEIGHT + 10.f));
	setSliderPosition(m_sliderPosition);
}

void Slider::update(const sf::Time& frameTime) {
	if (!m_isVisible || !m_isEnabled) return;

	m_knob.update(frameTime);
	if (m_knob.isPressed()) {
		handleDragAndDrop();
	}

	if (m_isSelected) {
		updateTime(m_scrollTimeout, frameTime);
		if (m_scrollTimeout == sf::Time::Zero && g_inputController->isLeft()) {
			setSliderPosition(m_sliderPosition - 1);
			m_scrollTimeout = sf::milliseconds(50);
		} 
		else if (m_scrollTimeout == sf::Time::Zero && g_inputController->isRight()) {
			setSliderPosition(m_sliderPosition + 1);
			m_scrollTimeout = sf::milliseconds(50);
		}
	}

	GameObject::update(frameTime);
}

void Slider::click() {
	int length = m_maxPosition - m_minPosition;
	if (m_sliderPosition == m_maxPosition) {
		setSliderPosition(m_minPosition);
	}
	else {
		setSliderPosition(m_sliderPosition + static_cast<int>(length / 10.f));
	}
}

void Slider::setText(const std::string& text) {
	setTextRaw(g_textProvider->getText(text));
}

void Slider::setTextRaw(const std::string& text) {
	m_title = text;
	m_titleText.setString(g_textProvider->getCroppedString(
		m_title + ": " + std::to_string(m_sliderPosition) + m_unit, GUIConstants::CHARACTER_SIZE_M, static_cast<int>(WIDTH)));

	setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
}

void Slider::setUnit(const std::string& unit) {
	m_unit = unit;
}

void Slider::setCharacterSize(int size) {
	m_titleText.setCharacterSize(size);
	float xOffset = WIDTH / 2.f - m_titleText.getLocalBounds().width / 2.f;
	float yOffset = -(10.f + m_titleText.getLocalBounds().height);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
	m_titleText.setPosition(getPosition() + m_textOffset);
}

void Slider::setEnabled(bool enabled) {
	ButtonInterface::setEnabled(enabled);
	m_filler.setFillColor(sf::Color(
		m_filler.getFillColor().r,
		m_filler.getFillColor().g,
		m_filler.getFillColor().b,
		m_isEnabled ? 255 : 100));
	m_titleText.setColorAlpha(m_isEnabled ? 255 : 100);
	m_minText.setColorAlpha(m_isEnabled ? 255 : 100);
	m_maxText.setColorAlpha(m_isEnabled ? 255 : 100);
}

// SLIDER KNOB

const float SliderKnob::WIDTH = 20.f;
const float SliderKnob::HEIGHT = 30.f;

SliderKnob::SliderKnob() : GameObject() {
	m_knob.setSize(sf::Vector2f(WIDTH, HEIGHT));
	m_knob.setTexture(g_resourceManager->getTexture(GlobalResource::TEX_GUI_KNOB));

	setBoundingBox(m_knob.getLocalBounds());
}

void SliderKnob::onLeftJustPressed() {
	m_isPressed = true;
	g_inputController->lockAction();
}

void SliderKnob::onLeftClick() {
	m_isPressed = false;
	g_inputController->lockAction();
}

bool SliderKnob::isPressed() const {
	return m_isPressed;
}

void SliderKnob::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_knob);
}

void SliderKnob::update(const sf::Time& frameTime) {
	GameObject::update(frameTime);
	if (m_isPressed && !g_inputController->isMousePressedLeft()) {
		m_isPressed = false;
	}
}

void SliderKnob::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos - sf::Vector2f(0.5f * WIDTH, 0.5f * HEIGHT));
	m_knob.setPosition(pos - sf::Vector2f(0.5f * WIDTH, 0.5f * HEIGHT));
}

GameObjectType SliderKnob::getConfiguredType() const {
	return _Button;
}