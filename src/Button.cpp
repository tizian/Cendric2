#include "Button.h"

using namespace std;

Button::Button(const sf::FloatRect& box)
{
	// using default values for constructor.
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));

	m_shape = sf::RectangleShape(sf::Vector2f(box.width, box.height));
	m_shape.setPosition(box.left, box.top);

	m_shape.setOutlineThickness(2.f);
	m_shape.setFillColor(CENDRIC_COLOR_BLACK);
	m_shape.setOutlineColor(CENDRIC_COLOR_PURPLE);

	m_pressedColor = CENDRIC_COLOR_LIGHT_PURPLE;
	m_mousoverColor = CENDRIC_COLOR_PURPLE;
	m_releasedColor = CENDRIC_COLOR_BLACK;
}

Button::Button(const sf::FloatRect& box, const sf::Color& pressedColor, const sf::Color& mousoverColor, const sf::Color& releasedColor)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));
	
	m_shape = sf::RectangleShape(sf::Vector2f(box.width, box.height));
	m_shape.setPosition(box.left, box.top);
	
	m_shape.setOutlineThickness(2.f);
	m_shape.setFillColor(releasedColor);
	m_shape.setOutlineColor(mousoverColor);

	m_pressedColor = pressedColor;
	m_mousoverColor = mousoverColor;
	m_releasedColor = releasedColor;
}

void Button::load() 
{
	// not used.
}

void Button::onLeftClick()
{
	if (m_isEnabled)
	{
		m_isPressed = true;
		m_shape.setFillColor(m_pressedColor);
		m_shape.setOutlineColor(m_mousoverColor);
	}
}

void Button::onMouseOver()
{
	if (m_isEnabled)
	{
		m_shape.setFillColor(m_mousoverColor);
		m_shape.setOutlineColor(m_releasedColor);
	}
}

void Button::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_shape);
	renderTarget.draw(m_text);
}

void Button::update(const sf::Time& frameTime)
{
	m_shape.setFillColor(m_isEnabled ? m_releasedColor : sf::Color(m_releasedColor.r, m_releasedColor.g, m_releasedColor.b, 100));
	m_shape.setOutlineColor(m_isEnabled ? m_mousoverColor : sf::Color(m_mousoverColor.r, m_mousoverColor.g, m_mousoverColor.b, 100));
	m_isPressed = false;
	GameObject::update(frameTime);
}

void Button::setText(Texts text, const sf::Color& color)
{
	m_text = sf::Text(
		g_textProvider->getText(text),
		(*g_resourceManager->getFont(ResourceID::Font_copperplateGothicBold)));
	
	m_text.setColor(color);
	// calculate position
	float xOffset = max((getBoundingBox()->width - m_text.getLocalBounds().width) / 2.f, 0.f);
	float yOffset = max((getBoundingBox()->height - (2 * m_text.getLocalBounds().height)) / 2.f, 0.f);
	m_text.setPosition(sf::Vector2f(xOffset, yOffset) + getPosition());
}

void Button::setText(Texts text)
{
	setText(text, CENDRIC_COLOR_LIGHT_PURPLE);
}

void Button::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
}

bool Button::isPressed() const
{
	return m_isPressed;
}

bool Button::isEnabled() const
{
	return m_isEnabled;
}

GameObjectType Button::getConfiguredType() const
{
	return GameObjectType::_Button;
}