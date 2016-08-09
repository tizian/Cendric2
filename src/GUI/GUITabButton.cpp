#include "GUI/GUITabButton.h"

#include "GlobalResource.h"
#include "Enums/EnumNames.h"
#include "GUI/GUIConstants.h"

const float GUITabButton::SIZE = 58.f;
const float GUITabButton::OFFSET = 4.f;

GUITabButton::GUITabButton(GUIElement element) {
	m_border.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_BUTTON_ROUND));
	m_icon.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_TAB_ICONS));

	m_background.setRadius(29.f);
	m_background.setFillColor(m_backgroundColor);
	
	int x = 50 * static_cast<int>(element);
	m_icon.setTextureRect(sf::IntRect(x, 0, 50, 50));

	setDebugBoundingBox(sf::Color::Blue);
	setInputInDefaultView(true);
	setBoundingBox(sf::FloatRect(0.f, 0.f, SIZE - 2 * OFFSET, SIZE - 2 * OFFSET));

	// agent placeholder
	m_executeOnClick = std::bind(&GUITabButton::nop, this);

	Key key;
	if (element == GUIElement::Character) {
		key = Key::CharacterInfo;
	}
	else if (element == GUIElement::Inventory) {
		key = Key::Inventory;
	}
	else if (element == GUIElement::Spellbook) {
		key = Key::Spellbook;
	}
	else if (element == GUIElement::Journal) {
		key = Key::Journal;
	}
	else if (element == GUIElement::Map) {
		key = Key::Map;
	}
	m_element = element;

	m_inputKey.setString(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[key]));
	m_inputKey.setCharacterSize((m_inputKey.getLocalBounds().width > SIZE - 10.f) ? 
		GUIConstants::CHARACTER_SIZE_S :
		GUIConstants::CHARACTER_SIZE_L);
}

void GUITabButton::setPosition(const sf::Vector2f& position) {
	sf::Vector2f innerPosition = position + sf::Vector2f(OFFSET, OFFSET);
	m_border.setPosition(position);
	m_icon.setPosition(innerPosition);
	m_background.setPosition(position);

	m_boundingBox.left = innerPosition.x;
	m_boundingBox.top = innerPosition.y;

	sf::Vector2f positionOffset(0.5f * (SIZE - m_inputKey.getLocalBounds().width), SIZE + 2.f);
	m_inputKey.setPosition(position + positionOffset);
}

void GUITabButton::update(const sf::Time& frameTime) {
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

void GUITabButton::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_background);
	renderTarget.draw(m_icon);
	renderTarget.draw(m_border);
	renderTarget.draw(m_inputKey);
}

void GUITabButton::setActive(bool active) {
	m_isActive = active;
	if (m_isActive) {
		m_background.setFillColor(m_activeColor);
		m_border.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_BUTTON_ROUND_SELECTED));
	}
	else {
		m_background.setFillColor(m_backgroundColor);
		m_border.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_GUI_BUTTON_ROUND));
	}
}

bool GUITabButton::isActive() const {
	return m_isActive;
}

void GUITabButton::onLeftJustPressed() {
	m_isPressed = true;
	g_inputController->lockAction();
}

void GUITabButton::onLeftClick() {
	if (m_isPressed) {
		m_isClicked = true;
		m_isPressed = false;
		g_inputController->lockAction();
	}
	if (!m_isActive) {
		m_background.setFillColor(m_backgroundColor);
	}
}

void GUITabButton::onMouseOver() {
	m_isMouseOver = true;
	if (!m_isActive) {
		m_background.setFillColor(m_highlightColor);
	}
}

bool GUITabButton::isClicked() const {
	return m_isClicked;
}

void GUITabButton::setOnClick(const std::function<void()>& agent) {
	m_executeOnClick = agent;
}

GUIElement GUITabButton::getElement() const {
	return m_element;
}

GameObjectType GUITabButton::getConfiguredType() const {
	return GameObjectType::_Button;
}

void GUITabButton::nop() const {

}