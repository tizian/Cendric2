#pragma once

#include "global.h"
#include "GUI/BitmapText.h"

class BitmapTextHolder final {
public:
	BitmapTextHolder(const std::string& str, const sf::Color& col, const sf::Vector2f& posOffset);
	void render(sf::RenderTarget& renderTarget);

	void setCharacterSize(int characterSize);
	void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPositionOffset() const;
	const sf::Vector2f& getSize() const;

private:
	BitmapText m_text;
	sf::Vector2f m_positionOffset;
	sf::Vector2f m_size;
};