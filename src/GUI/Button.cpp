#include "GUI/Button.h"

using namespace std;

Button::Button(const sf::FloatRect& box)
{
	// using default values for constructor.
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(box);
	setPosition(sf::Vector2f(box.left, box.top));

	m_sprite = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_button_sliced), box.width, box.height);
	m_spritePressed = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_button_sliced_pressed), box.width, box.height);
	m_spriteMouseover = SlicedSprite(g_resourceManager->getTexture(ResourceID::Texture_GUI_button_sliced_mouseover), box.width, box.height);

	m_sprite.setSlicing(21, 21, 22, 22);
	m_spritePressed.setSlicing(21, 21, 22, 22);
	m_spriteMouseover.setSlicing(21, 21, 22, 22);

	m_sprite.setPosition(box.left, box.top);
	m_spritePressed.setPosition(box.left, box.top);
	m_spriteMouseover.setPosition(box.left, box.top);

	m_state = ButtonState::DEFAULT;
}

void Button::load() 
{
	// not used.
}

void Button::onLeftClick()
{
	if (m_isEnabled && m_isPressed)
	{
		m_isClicked = true;
		m_isPressed = false;
		m_state = ButtonState::PRESSED;
	}
}

void Button::onLeftJustPressed()
{
	if (m_isEnabled)
	{
		m_isPressed = true;
		m_state = ButtonState::PRESSED;
	}
}

void Button::onMouseOver()
{
	if (m_isEnabled && !m_isPressed)
	{
		m_state = ButtonState::MOUSEOVER;
	}
}

void Button::render(sf::RenderTarget& renderTarget)
{
	if (m_state == ButtonState::DEFAULT) {
		renderTarget.draw(m_sprite);
	}
	else if (m_state == ButtonState::MOUSEOVER) {
		renderTarget.draw(m_spriteMouseover);
	}
	else if (m_state == ButtonState::PRESSED) {
		renderTarget.draw(m_spritePressed);
	}
	
	renderTarget.draw(m_text);
}

void Button::update(const sf::Time& frameTime)
{
	m_isClicked = false;
	if (!g_inputController->isMouseOver(getBoundingBox()))
	{
		m_isPressed = false;
		m_state = ButtonState::DEFAULT;
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
	m_text.setPosition(sf::Vector2f(xOffset, yOffset) + getPosition());
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
	m_text.setPosition(sf::Vector2f(xOffset, yOffset) + getPosition());
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
	m_text.setPosition(sf::Vector2f(xOffset, yOffset) + getPosition());
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