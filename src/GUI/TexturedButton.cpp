#include "GUI/TexturedButton.h"

TexturedButton::TexturedButton(const sf::FloatRect& box, ButtonOrnamentStyle style) : Button(box, style)
{
	m_texturedLayer.setSize(sf::Vector2f(m_boundingBox.width, m_boundingBox.height));
	m_texturedLayer.setPosition(getPosition());
}

void TexturedButton::render(sf::RenderTarget& renderTarget)
{
	renderTarget.draw(m_backLayer);
	renderTarget.draw(m_mainLayer);
	renderTarget.draw(m_texturedLayer);
	renderTarget.draw(m_ornamentLayer);
	renderTarget.draw(m_text);
}

void TexturedButton::setPosition(const sf::Vector2f& pos)
{
	Button::setPosition(pos);
	m_texturedLayer.setPosition(pos);
}

void TexturedButton::setTexture(const sf::Texture* tex, const sf::IntRect& textureRect)
{
	m_texturedLayer.setTexture(tex);
	m_texturedLayer.setTextureRect(textureRect);
}

void TexturedButton::setTextureColor(const sf::Color& color)
{
	m_texturedLayer.setFillColor(color);
}
