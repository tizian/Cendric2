#include "GUI/Button.h"
#include "GlobalResource.h"
#include "Enums/EnumNames.h"
#include "Controller/GamepadMappings.h"

Button::Button(const sf::FloatRect& box, GUIOrnamentStyle style) :
	GameObject(),
	m_backLayerColor(COLOR_DARK_GREY),
	m_mainLayerColor(COLOR_BLACK),
	m_ornamentLayerColor(COLOR_WHITE),
	m_mouseOverColor(COLOR_PURPLE),
	m_selectedColor(COLOR_MEDIUM_PURPLE) {

	setBoundingBox(box);
	setInputInDefaultView(true);

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ROUNDED_RECTANGLE), m_mainLayerColor, box.width, box.height);

	m_backLayer = SlicedSprite(g_resourceManager->getTexture(GlobalResource::TEX_GUI_ROUNDED_RECTANGLE), m_backLayerColor, box.width, box.height);
	m_backLayerOffset = sf::Vector2f(0, 2);

	m_ornamentLayer = SlicedSprite(getOrnamentStyleTexture(style), m_ornamentLayerColor, box.width, box.height);

	m_positionDefault = sf::Vector2f(box.left, box.top);
	setPosition(m_positionDefault);

	m_keyText.setColor(COLOR_LIGHT_PURPLE);
}

void Button::onLeftClick() {
	if (m_isEnabled && m_isPressed) {
		m_isClicked = true;
		m_isPressed = false;
		m_mainLayer.move(0, 1);
		m_ornamentLayer.move(0, 1);
		m_text.move(0, 1);
		m_textureLayer.move(0, 1);
		updateColor();
		g_inputController->lockAction();
	}
}

void Button::onLeftJustPressed() {
	if (m_isEnabled) {
		m_isPressed = true;
		m_mainLayer.setPosition(m_positionDefault);
		m_ornamentLayer.setPosition(m_positionDefault);
		m_mainLayer.move(0, 1);
		m_ornamentLayer.move(0, 1);
		m_text.move(0, 1);
		m_textureLayer.move(0, 1);
		updateColor();
		g_inputController->lockAction();
	}
}

void Button::onMouseOver() {
	m_isMouseOver = true;
	updateColor();
}

void Button::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_mainLayer);
	renderTarget.draw(m_ornamentLayer);
	renderTarget.draw(m_text);
	if (hasGamepadKey()) {
		reloadTextPosition();
		renderTarget.draw(m_keyText);
	}
	if (m_hasTexture) renderTarget.draw(m_textureLayer);
}

void Button::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_mainLayer.setPosition(pos);
	m_backLayer.setPosition(pos + m_backLayerOffset);
	m_ornamentLayer.setPosition(pos);
	m_text.setPosition(pos + m_textOffset);
	m_keyText.setPosition(pos + m_keyTextOffset);
	m_textureLayer.setPosition(pos + m_textureOffset);
	m_positionDefault = pos;
}

void Button::setSize(const sf::Vector2f& size) {
	GameObject::setSize(size);
	m_ornamentLayer.setSize(size);
	m_backLayer.setSize(size);
	m_mainLayer.setSize(size);

	// this re-centers the text
	setCharacterSize(m_text.getCharacterSize());
}

void Button::update(const sf::Time& frameTime) {
	if (!m_isVisible || !m_isEnabled) return;

	if (m_isMouseOver && !(g_inputController->isMouseOver(getBoundingBox(), true))) {
		m_isMouseOver = false;
		m_isPressed = false;
		updateColor();
		setPosition(m_positionDefault);
	}
	if (!m_isPressed && m_isClicked) {
		setPosition(m_positionDefault);
	}
	m_isClicked = false;
	GameObject::update(frameTime);
	if (m_isClicked || (hasGamepadKey() && g_inputController->isJoystickButtonJustPressed(m_gamepadKey))) {
		click();
	}
}

void Button::setText(const std::string& text, const sf::Color& color, int charSize) {
	setTextRaw(g_textProvider->getText(text), color, charSize);
}

void Button::setText(const std::string& text) {
	setText(text, COLOR_WHITE, 16);
}

void Button::setText(const std::string& text, int charSize) {
	setText(text, COLOR_WHITE, charSize);
}

void Button::setTextRaw(const std::string& text, const sf::Color& color, int charSize) {
	m_text = BitmapText(text);
	float usedWidth = charSize * static_cast<int>(text.length()) + 30.f;
	if (usedWidth > m_boundingBox.width) {
		setSize(sf::Vector2f(usedWidth, m_boundingBox.height));
	}

	setTextColor(color);
	setCharacterSize(charSize);
}

void Button::setTextRaw(const std::string& text, int charSize) {
	setTextRaw(text, COLOR_WHITE, charSize);
}

void Button::setTextRaw(const std::string& text) {
	setTextRaw(text, COLOR_WHITE, 16);
}

void Button::setTexture(const sf::Texture* texture) {
	if (texture == nullptr) {
		m_hasTexture = false;
		return;
	}
	m_hasTexture = true;

	sf::Vector2f textureSize(static_cast<float>(texture->getSize().x), static_cast<float>(texture->getSize().y));
	sf::Vector2f pos = getPosition();
	sf::Vector2f size = getSize();

	m_textureLayer.setTexture(*texture);
	m_textureOffset = 0.5f * (size - textureSize);
	m_textureLayer.setPosition(pos + m_textureOffset);
}

void Button::setTextureColor(const sf::Color& color) {
	m_textureLayer.setColor(color);
}

void Button::setCharacterSize(int size) {
	m_text.setCharacterSize(size);
	m_keyText.setCharacterSize(size);
	m_characterSize = size;
	reloadTextPosition();
}

void Button::reloadTextPosition() {
	float textWidth = hasGamepadKey() ?
		m_text.getLocalBounds().width + m_keyText.getLocalBounds().width :
		m_text.getLocalBounds().width;

	float xOffset = std::max((getBoundingBox()->width - textWidth) * 0.5f, 0.f);
	float yOffset = std::max((getBoundingBox()->height - m_text.getLocalBounds().height) * 0.5f, 0.f);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
	m_text.setPosition(getPosition() + m_textOffset);
	m_keyTextOffset = m_textOffset + sf::Vector2f(m_text.getLocalBounds().width, 0.f);
	m_keyText.setPosition(getPosition() + m_keyTextOffset);
}

void Button::setBackgroundLayerColor(const sf::Color& color) {
	m_backLayer.setColor(color);
	m_backLayerColor = color;
}

void Button::setTextColor(const sf::Color& color) {
	m_text.setColor(color);
}

void Button::setMouseOverColor(const sf::Color& color) {
	m_mouseOverColor = color;
}

void Button::setSelectedColor(const sf::Color& color) {
	m_selectedColor = color;
}

void Button::setMainLayerColor(const sf::Color& color) {
	m_mainLayer.setColor(color);
	m_mainLayerColor = color;
}

void Button::setOrnamentLayerColor(const sf::Color& color) {
	m_ornamentLayer.setColor(color);
	m_ornamentLayerColor = color;
}

sf::Color Button::getBackgroundLayerColor() const {
	return m_backLayerColor;
}

sf::Color Button::getMainLayerColor() const {
	return m_mainLayerColor;
}

sf::Color Button::getOrnamentLayerColor() const {
	return m_ornamentLayerColor;
}

sf::Color Button::getMouseOverColor() const {
	return m_mouseOverColor;
}

void Button::setOrnamentLayerTexture(sf::Texture* texture) {
	m_ornamentLayer.setTexture(texture);
}

void Button::setEnabled(bool enabled) {
	ButtonInterface::setEnabled(enabled);
	m_text.setColor(sf::Color(m_text.getColor().r, m_text.getColor().g, m_text.getColor().b, m_isEnabled ? 255 : 100));
	m_textureLayer.setColor(sf::Color(m_text.getColor().r, m_text.getColor().g, m_text.getColor().b, m_isEnabled ? 255 : 100));
}

void Button::updateColor() {
	m_mainLayer.setColor(m_isMouseOver && !m_isPressed ?
		m_mouseOverColor :
		m_isSelected ? m_selectedColor : m_mainLayerColor);
}

void Button::setOnClick(const std::function<void()>& agent) {
	m_executeOnClick = agent;
}

void Button::setGamepadKey(Key key) {
	m_gamepadKey = key;
	m_keyText.setString(" (" + GamepadMappings::getKeyName(key) + ")");
	reloadTextPosition();
}

bool Button::hasGamepadKey() const {
	return m_gamepadKey != Key::VOID && g_inputController->isGamepadConnected();
}

void Button::click() {
	m_isClicked = true;
	m_executeOnClick();
	g_inputController->lockAction();
}