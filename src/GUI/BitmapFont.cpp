#include "GUI/BitmapFont.h"

BitmapFont::BitmapFont() {}

bool BitmapFont::loadFromFile(const std::string &path) {
	bool loaded = m_texture.loadFromFile(path);

	if (!loaded) return false;
	int width = m_texture.getSize().x;
	int height = m_texture.getSize().y;
	if (width % 16 != 0 || height % 14 != 0) return false;
	
	m_glyphSize.x = width / 16;
	m_glyphSize.y = height / 14;

	return true;
}

const sf::Texture &BitmapFont::getTexture() const {
	return m_texture;
}

const sf::Vector2i &BitmapFont::getGlyphSize() const {
	return m_glyphSize;
}