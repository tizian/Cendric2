#include "GUI/Button.h"

using namespace std;

Button::Button(const sf::FloatRect& box, ButtonOrnamentStyle style)
{
	// using default values for constructor.
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(box);
	m_positionDefault = sf::Vector2f(box.left, box.top);
	setPosition(m_positionDefault);

	m_mainLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), CENDRIC_COLOR_BLACK, box.width, box.height);
	m_mainLayer.setPosition(m_positionDefault);

	m_backLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_rounded_rectangle), CENDRIC_COLOR_LIGHT_PURPLE, box.width, box.height);
	m_backLayerOffset = sf::Vector2f(0, 2);
	m_backLayer.setPosition(m_positionDefault + m_backLayerOffset);

	if (style == ButtonOrnamentStyle::NONE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_none), CENDRIC_COLOR_WHITE, box.width, box.height);
	}
	else if (style == ButtonOrnamentStyle::SMALL) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_small), CENDRIC_COLOR_WHITE, box.width, box.height);
	}
	else if (style == ButtonOrnamentStyle::MEDIUM) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_medium), CENDRIC_COLOR_WHITE, box.width, box.height);
	}
	else if (style == ButtonOrnamentStyle::LARGE) {
		m_ornamentLayer = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_ornament_large), CENDRIC_COLOR_WHITE, box.width, box.height);
	}

	m_ornamentLayer.setPosition(m_positionDefault);
}

void Button::onLeftClick()
{
	if (m_isEnabled && m_isPressed)
	{
		m_isClicked = true;
		m_isPressed = false;
		m_mainLayer.move(0, 1);
		m_mainLayer.setColor(CENDRIC_COLOR_LIGHT_PURPLE);
	}
}

void Button::onLeftJustPressed()
{
	if (m_isEnabled)
	{
		m_isPressed = true;
		m_mainLayer.move(0, 1);
		m_mainLayer.setColor(CENDRIC_COLOR_LIGHT_PURPLE);
	}
}

void Button::onMouseOver()
{
	if (m_isEnabled && !m_isPressed)
	{
		m_mainLayer.setColor(CENDRIC_COLOR_PURPLE);
	}
}

void Button::render(sf::RenderTarget& renderTarget)
{
	m_ornamentLayer.setPosition(m_mainLayer.getPosition());
	m_text.setPosition(m_mainLayer.getPosition() + m_textOffset);
	
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_mainLayer);
	renderTarget.draw(m_ornamentLayer);
	renderTarget.draw(m_text);
}

void Button::update(const sf::Time& frameTime)
{
	m_isClicked = false;
	if (!g_inputController->isMouseOver(getBoundingBox()))
	{
		m_isPressed = false;
		m_mainLayer.setPosition(m_positionDefault);
		m_mainLayer.setColor(CENDRIC_COLOR_BLACK);
	}
	GameObject::update(frameTime);
}

void Button::setText(const std::string& text, const sf::Color& color, int charSize)
{
	m_text = BitmapText(
		g_textProvider->getText(text),
		*g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default));
	
	m_text.setColor(color);
	m_text.setCharacterSize(charSize);
	// calculate position
	float xOffset = max((getBoundingBox()->width - m_text.getLocalBounds().width) / 2.f, 0.f);
	float yOffset = max((getBoundingBox()->height - m_text.getLocalBounds().height) / 2.f , 0.f);
	m_textOffset = sf::Vector2f(xOffset, yOffset);
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
	m_text = BitmapText(
		text,
		*g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default));

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

void Button::setTextColor(const sf::Color& color)
{
	m_text.setColor(color);
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