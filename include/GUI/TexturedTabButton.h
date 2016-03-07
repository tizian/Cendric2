#pragma once

#include "global.h"
#include "TabButton.h"

class TexturedTabButton : public TabButton {
public:
	TexturedTabButton(const sf::FloatRect& box);
	void render(sf::RenderTarget& renderTarget) override;
	void setPosition(const sf::Vector2f& pos) override;
	void setTexture(const sf::Texture* tex, const sf::IntRect& textureRect);
	void setTextureColor(const sf::Color& color);

private:
	sf::RectangleShape m_texturedLayer;
};