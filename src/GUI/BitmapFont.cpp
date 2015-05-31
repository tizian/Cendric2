#include "GUI/BitmapFont.h"

BitmapFont::BitmapFont() {}

bool BitmapFont::loadFromFile(const std::string &path, int glyphWidth, int glyphHeight)
{
	bool loaded = m_texture.loadFromFile(path);

	if (!loaded || glyphWidth < 0 || glyphHeight < 0) return false;

	m_glyphSize.x = glyphWidth;
	m_glyphSize.y = glyphHeight;

	return true;
}

const sf::Texture &BitmapFont::getTexture() const
{
	return m_texture;
}

const sf::Vector2i &BitmapFont::getGlyphSize() const
{
	return m_glyphSize;
}