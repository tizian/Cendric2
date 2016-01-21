#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "global.h"
#include <string>

class BitmapFont {
public:
	BitmapFont();

	bool loadFromFile(const std::string &path);

	const sf::Texture &getTexture() const;

	const sf::Vector2i &getGlyphSize() const;

private:
	sf::Texture m_texture;
	sf::Vector2i m_glyphSize;
};