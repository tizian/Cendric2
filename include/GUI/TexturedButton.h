#pragma once

#include "global.h"
#include "Button.h"

// A button that takes a texture additionally.
class TexturedButton : public Button
{
public:
	TexturedButton(const sf::FloatRect& box, ButtonOrnamentStyle style = ButtonOrnamentStyle::NONE);
	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& pos) override;
	void setTexture(const sf::Texture* tex, const sf::IntRect& textureRect);
	void setTextureColor(const sf::Color& color);

private:
	sf::RectangleShape m_texturedLayer;
};