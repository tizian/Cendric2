#include "GUI/ArrowSelector.h"

using namespace std;

ArrowSelector::ArrowSelector() : GameObject(),
m_button(sf::FloatRect(0.f, 0.f, 50.f, 50.f)),
m_rightArrow(true),
m_leftArrow(false) {

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

void ArrowSelector::addOptionRaw(const std::string& option) {
	m_options.push_back(option);
	BitmapText dummy;
	dummy.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	dummy.setString(option);
	float usedWidth = dummy.getLocalBounds().width + 4 * GUIConstants::BUTTON_MARGIN;
	if (usedWidth > m_button.getBoundingBox()->width) {
		m_button.setSize(sf::Vector2f(usedWidth, m_button.getBoundingBox()->height));
		recalculatePosition();
	}
	if (m_chosenOptionIndex < 0) {
		setOptionIndex(0);
	}
}

void ArrowSelector::setOptionIndex(int index) {
	if (index < 0 || index + 1 > m_options.size()) return;
	m_chosenOptionIndex = index;
	m_rightArrow.setEnabled(m_chosenOptionIndex + 1 < m_options.size());
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
	m_rightArrow.setEnabled(enabled && m_chosenOptionIndex + 1 < m_options.size());
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
	return GameObjectType::_Button;
}

// ARROW SELECTOR ARROW

const sf::Color ArrowSelectorArrow::MOUSEOVER_COLOR = CENDRIC_COLOR_PURPLE;
const sf::Color ArrowSelectorArrow::MAIN_COLOR = CENDRIC_COLOR_WHITE;
const sf::Color ArrowSelectorArrow::DISABLED_COLOR = CENDRIC_COLOR_DARK_GREY;

ArrowSelectorArrow::ArrowSelectorArrow(bool pointRight) : GameObject() {
	m_arrow.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_GUI_arrow));

	// center origin
	m_arrow.setOrigin(sf::Vector2f(m_arrow.getLocalBounds().width / 2.f, m_arrow.getLocalBounds().height / 2.f));
	m_arrow.rotate(pointRight ? 90.f : -90.f);
	setBoundingBox(sf::FloatRect(0.f, 0.f, m_arrow.getLocalBounds().height, m_arrow.getLocalBounds().width));
}

void ArrowSelectorArrow::onLeftClick() {
	if (!m_isEnabled) return;
	m_isClicked = true;
	g_inputController->lockAction();
}

bool ArrowSelectorArrow::isClicked() const {
	return m_isClicked;
}

void ArrowSelectorArrow::onMouseOver() {
	if (!m_isEnabled) return;
	m_isMouseOver = true;
}

void ArrowSelectorArrow::setEnabled(bool value) {
	m_isEnabled = value;
	m_arrow.setColor(m_isEnabled ? MAIN_COLOR : DISABLED_COLOR);
}

void ArrowSelectorArrow::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_arrow);
}

void ArrowSelectorArrow::update(const sf::Time& frameTime) {
	m_isClicked = false;
	if (m_isMouseOver != m_wasMouseOver) {
		m_arrow.setColor(m_isMouseOver ? MOUSEOVER_COLOR : m_isEnabled ? MAIN_COLOR : DISABLED_COLOR);
	}
	m_wasMouseOver = m_isMouseOver;
	m_isMouseOver = false;
	GameObject::update(frameTime);
}

void ArrowSelectorArrow::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_arrow.setPosition(pos + 0.5f * getSize());
}

GameObjectType ArrowSelectorArrow::getConfiguredType() const {
	return GameObjectType::_Button;
}