#include "GUI/TabButton.h"
#include "GlobalResource.h"

const float TabButton::BORDER_OFFSET = 5.f;
const float TabButton::ALIGNMENT_OFFSET = 14.f;

TabButton::TabButton(const sf::FloatRect& box) {
	// using default values for constructor.
	m_border = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_INACTIVE), COLOR_WHITE, box.width, box.height);
	m_border.setPosition(box.left, box.top);

	m_outerRect = box;
	sf::FloatRect innerBox = sf::FloatRect(box.left + BORDER_OFFSET, box.top + BORDER_OFFSET, box.width - 2.f * BORDER_OFFSET, box.height - 2.f * BORDER_OFFSET);

	m_background = sf::RectangleShape();
	m_background.setPosition(sf::Vector2f(innerBox.left, innerBox.top));
	m_background.setSize(sf::Vector2f(innerBox.width, innerBox.height));
	m_background.setFillColor(m_backgroundColor);

	setBoundingBox(innerBox);
	setInputInDefaultView(true);

	setPosition(sf::Vector2f(box.left, box.top));

	m_isActive = false;

	// agent placeholder
	m_executeOnClick = std::bind(&TabButton::nop, this);
}

void TabButton::setActive(bool active) {
	m_isActive = active;
	if (m_isActive) {
		m_background.setFillColor(m_activeColor);
	}
	else {
		m_background.setFillColor(m_backgroundColor);
	}
}

bool TabButton::isActive() const {
	return m_isActive;
}

void TabButton::onLeftClick() {
	if (m_isPressed) {
		m_isClicked = true;
		m_isPressed = false;
		g_inputController->lockAction();
	}
	if (!m_isActive) {
		m_background.setFillColor(m_backgroundColor);
	}
}

void TabButton::onLeftJustPressed() {
	m_isPressed = true;
	g_inputController->lockAction();
}

void TabButton::onMouseOver() {
	m_isMouseOver = true;
	if (!m_isActive) {
		m_background.setFillColor(m_highlightColor);
	}
}

void TabButton::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_background);
	renderTarget.draw(m_border);
	renderTarget.draw(m_text);
}

void TabButton::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);

	m_border.setPosition(pos);

	m_outerRect.left = pos.x;
	m_outerRect.top = pos.y;
	sf::Vector2f innerPos(pos.x + BORDER_OFFSET, pos.y + BORDER_OFFSET);
	m_background.setPosition(innerPos);

	m_text.setPosition(pos + m_textOffset);
}

void TabButton::setSize(const sf::Vector2f& size) {
	GameObject::setSize(size);
	m_outerRect.width = size.x;
	m_outerRect.height = size.y;
	m_border.setSize(size);
	sf::Vector2f innerSize(size.x - 2.f * BORDER_OFFSET, size.y - 2.f * BORDER_OFFSET);
	m_background.setSize(innerSize);
	
	// this re-centers the text
	setCharacterSize(m_text.getCharacterSize());
}

void TabButton::update(const sf::Time& frameTime) {
	if (m_isMouseOver && !(g_inputController->isMouseOver(getBoundingBox(), true))) {
		m_isMouseOver = false;
		m_isPressed = false;
		if (!m_isActive) {
			m_background.setFillColor(m_backgroundColor);
		}
	}
	m_isClicked = false;
	GameObject::update(frameTime);
	if (m_isClicked) {
		m_executeOnClick();
	}
}

void TabButton::setText(const std::string& text, const sf::Color& color, int charSize) {
	m_text = BitmapText(g_textProvider->getText(text));

	setTextColor(color);
	setCharacterSize(charSize);
}

void TabButton::setText(const std::string& text) {
	setText(text, COLOR_WHITE, 16);
}

void TabButton::setText(const std::string& text, int charSize) {
	setText(text, COLOR_WHITE, charSize);
}

void TabButton::setTextRaw(const std::string& text, const sf::Color& color, int charSize) {
	m_text = BitmapText(text);

	setTextColor(color);
	setCharacterSize(charSize);
}

void TabButton::setTextRaw(const std::string& text, int charSize) {
	setTextRaw(text, COLOR_WHITE, charSize);
}

void TabButton::setTextRaw(const std::string& text) {
	setTextRaw(text, COLOR_WHITE, 16);
}

void TabButton::setCharacterSize(int size) {
	m_text.setCharacterSize(size);
	float xOffset = std::max(0.5f * (m_outerRect.width - m_text.getLocalBounds().width), 0.f);
	float yOffset = std::max(0.5f * (m_outerRect.height - m_text.getLocalBounds().height), 0.f);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
	m_text.setPosition(getPosition() + m_textOffset);
}

void TabButton::setTextColor(const sf::Color& color) {
	m_text.setColor(color);
}

void TabButton::setBackgroundColor(const sf::Color& color) {
	m_backgroundColor = color;
}

void TabButton::setHighlightColor(const sf::Color& color) {
	m_highlightColor = color;
}

bool TabButton::isClicked() const {
	return m_isClicked;
}

GameObjectType TabButton::getConfiguredType() const {
	return GameObjectType::_Button;
}

void TabButton::setOnClick(const std::function<void()>& agent) {
	m_executeOnClick = agent;
}

void TabButton::nop() const {
	// nop
}