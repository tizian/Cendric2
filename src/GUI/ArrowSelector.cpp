#include "GUI/ArrowSelector.h"

using namespace std;

ArrowSelector::ArrowSelector() : GameObject(),
m_rightArrow(true),
m_leftArrow(false),
m_button(sf::FloatRect(0.f, 0.f, 50.f, 50.f)) {

	m_label.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_button.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	setInputInDefaultView(true);
	setBoundingBox(*m_button.getBoundingBox());
	m_leftArrow.setEnabled(false);
	m_rightArrow.setEnabled(false);
}

int ArrowSelector::getChosenOptionIndex() const {
	return m_chosenOptionIndex;
}

void ArrowSelector::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	m_button.render(renderTarget);
	m_leftArrow.render(renderTarget);
	m_rightArrow.render(renderTarget);
	renderTarget.draw(m_label);
}

void ArrowSelector::recalculatePosition() {
	sf::Vector2f pos = getPosition();
	m_button.setPosition(sf::Vector2f(pos.x + GUIConstants::LABEL_OFFSET, pos.y));
	float buttonHalf = m_button.getBoundingBox()->height / 2.f;
	m_label.setPosition(sf::Vector2f(pos.x, pos.y +
		buttonHalf - m_label.getLocalBounds().height / 2.f));
	m_leftArrow.setPosition(m_button.getPosition() + sf::Vector2f(
		-GUIConstants::BUTTON_MARGIN - m_leftArrow.getBoundingBox()->width,
		buttonHalf - m_leftArrow.getBoundingBox()->height / 2.f));
	m_rightArrow.setPosition(m_button.getPosition() + sf::Vector2f(
		GUIConstants::BUTTON_MARGIN + m_button.getBoundingBox()->width,
		buttonHalf - m_rightArrow.getBoundingBox()->height / 2.f));
}

void ArrowSelector::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	recalculatePosition();
}

void ArrowSelector::update(const sf::Time& frameTime) {
	if (!m_isVisible || !m_isEnabled) return;

	m_leftArrow.update(frameTime);
	m_rightArrow.update(frameTime);
	if (m_leftArrow.isClicked()) {
		setOptionIndex(m_chosenOptionIndex - 1);
	}
	else if (m_rightArrow.isClicked()) {
		setOptionIndex(m_chosenOptionIndex + 1);
	}
}

void ArrowSelector::addOption(const std::string& optionKey) {
	addOptionRaw(g_textProvider->getText(optionKey));
}

void ArrowSelector::addOptionRaw(const std::string& rawOption) {
	m_options.push_back(rawOption);
	float usedWidth = GUIConstants::CHARACTER_SIZE_M * static_cast<int>(rawOption.length()) + 30.f;
	if (usedWidth > m_button.getBoundingBox()->width) {
		m_button.setSize(sf::Vector2f(usedWidth, m_button.getBoundingBox()->height));
		recalculatePosition();
	}
	if (m_chosenOptionIndex < 0) {
		setOptionIndex(0);
	}
}

void ArrowSelector::setOptionIndex(int index) {
	if (index < 0 || index + 1 > static_cast<int>(m_options.size())) return;
	m_chosenOptionIndex = index;
	m_rightArrow.setEnabled(m_chosenOptionIndex + 1 < static_cast<int>(m_options.size()));
	m_leftArrow.setEnabled(m_chosenOptionIndex > 0);
	m_button.setTextRaw(m_options.at(m_chosenOptionIndex), GUIConstants::CHARACTER_SIZE_M);
}

void ArrowSelector::setLabelText(const std::string& text) {
	setLabelTextRaw(g_textProvider->getCroppedText(text, GUIConstants::CHARACTER_SIZE_M, 
		static_cast<int>(GUIConstants::LABEL_OFFSET - m_rightArrow.getBoundingBox()->width)));
}

void ArrowSelector::setLabelTextRaw(const std::string& text) {
	m_label.setString(text);
}

void ArrowSelector::setEnabled(bool enabled) {
	m_isEnabled = enabled;
	m_leftArrow.setEnabled(enabled && m_chosenOptionIndex > 0);
	m_rightArrow.setEnabled(enabled && m_chosenOptionIndex + 1 < static_cast<int>(m_options.size()));
}

void ArrowSelector::setVisible(bool value) {
	m_isVisible = value;
}

bool ArrowSelector::isEnabled() const {
	return m_isEnabled;
}

bool ArrowSelector::isVisible() const {
	return m_isVisible;
}

GameObjectType ArrowSelector::getConfiguredType() const {
	return _Button;
}
