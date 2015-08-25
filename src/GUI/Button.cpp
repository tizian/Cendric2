#include "GUI/Button.h"

using namespace std;

Button::Button(const sf::FloatRect& box, ButtonOrnamentStyle style) : GameObject()
{
	// using default values for constructor.
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(box);
	setInputInDefaultView(true);

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), m_mainLayerColor, box.width, box.height);

	m_backLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), m_backLayerColor, box.width, box.height);
	m_backLayerOffset = sf::Vector2f(0, 2);

	if (style == ButtonOrnamentStyle::NONE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none), m_ornamentLayerColor, box.width, box.height);
	}
	else if (style == ButtonOrnamentStyle::SMALL) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_small), sf::Color::White, box.width, box.height);
	}
	else if (style == ButtonOrnamentStyle::MEDIUM) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_medium), sf::Color::White, box.width, box.height);
	}
	else if (style == ButtonOrnamentStyle::LARGE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_large), sf::Color::White, box.width, box.height);
	}

	m_positionDefault = sf::Vector2f(box.left, box.top);
	setPosition(m_positionDefault);
}

void Button::onLeftClick()
{
	if (m_isEnabled && m_isPressed)
	{
		m_isClicked = true;
		m_isPressed = false;
		m_mainLayer.move(0, 1);
		m_ornamentLayer.move(0, 1);
		m_mainLayer.setColor(m_backLayerColor);
		g_inputController->lockAction();
	}
}

void Button::onLeftJustPressed()
{
	if (m_isEnabled)
	{
		m_isPressed = true;
		m_mainLayer.move(0, 1);
		m_ornamentLayer.move(0, 1);
		m_mainLayer.setColor(m_backLayerColor);
		g_inputController->lockAction();
	}
}

void Button::onMouseOver()
{
	m_isMouseOver = true;
	if (m_isEnabled && !m_isPressed)
	{
		m_mainLayer.setColor(m_mouseOverColor);
	}
}

void Button::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_mainLayer);
	renderTarget.draw(m_ornamentLayer);
	renderTarget.draw(m_text);
}

void Button::setPosition(const sf::Vector2f& pos)
{
	GameObject::setPosition(pos);
	m_mainLayer.setPosition(pos);
	m_backLayer.setPosition(pos + m_backLayerOffset);
	m_ornamentLayer.setPosition(pos);
	m_text.setPosition(pos + m_textOffset);
	m_positionDefault = pos;
}

void Button::update(const sf::Time& frameTime)
{
	m_isClicked = false;
	if (m_isMouseOver && !(g_inputController->isMouseOver(getBoundingBox(), true)))
	{
		m_isPressed = false;
		m_isMouseOver = false;
		m_mainLayer.setColor(m_mainLayerColor);
		setPosition(m_positionDefault);
	}
	GameObject::update(frameTime);
}

void Button::setText(const std::string& text, const sf::Color& color, int charSize)
{
	m_text = BitmapText(g_textProvider->getText(text));
	
	m_text.setColor(color);
	m_text.setCharacterSize(charSize);
	// calculate position
	float xOffset = max((getBoundingBox()->width - m_text.getLocalBounds().width) / 2.f, 0.f);
	float yOffset = max((getBoundingBox()->height - m_text.getLocalBounds().height) / 2.f , 0.f);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
	setPosition(m_positionDefault);
}

void Button::setText(const std::string& text)
{
	setText(text, sf::Color::White, 16);
}

void Button::setText(const std::string& text, int charSize)
{
	setText(text, sf::Color::White, charSize);
}

void Button::setTextRaw(const std::string& text, const sf::Color& color, int charSize)
{
	m_text = BitmapText(text);

	m_text.setColor(color);
	m_text.setCharacterSize(charSize);
	// calculate position
	float xOffset = max((getBoundingBox()->width - m_text.getLocalBounds().width) / 2.f, 0.f);
	float yOffset = max((getBoundingBox()->height - m_text.getLocalBounds().height) / 2.f, 0.f);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
}

void Button::setTextRaw(const std::string& text, int charSize)
{
	setTextRaw(text, sf::Color::White, charSize);
}

void Button::setTextRaw(const std::string& text)
{
	setTextRaw(text, sf::Color::White, 16);
}

void Button::setCharacterSize(int size)
{
	m_text.setCharacterSize(size);
	float xOffset = max((getBoundingBox()->width - m_text.getLocalBounds().width) / 2.f, 0.f);
	float yOffset = max((getBoundingBox()->height - m_text.getLocalBounds().height) / 2.f, 0.f);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
}

void Button::setBackgroundLayerColor(const sf::Color& color)
{
	m_backLayer.setColor(color);
	m_backLayerColor = color;
}

void Button::setTextColor(const sf::Color& color)
{
	m_text.setColor(color);
}

void Button::setMouseOverColor(const sf::Color& color)
{
	m_mouseOverColor = color;
}

void Button::setMainLayerColor(const sf::Color& color)
{
	m_mainLayer.setColor(color);
	m_mainLayerColor = color;
}

void Button::setOrnamentLayerColor(const sf::Color& color)
{
	m_ornamentLayer.setColor(color);
	m_ornamentLayerColor = color;
}

void Button::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
	m_text.setColor(sf::Color(m_text.getColor().r, m_text.getColor().g, m_text.getColor().b, m_isEnabled ? 255 : 100));
}

bool Button::isClicked() const
{
	return m_isClicked;
}

bool Button::isEnabled() const
{
	return m_isEnabled;
}

GameObjectType Button::getConfiguredType() const
{
	return GameObjectType::_Button;
}