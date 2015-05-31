#include "GUI/BitmapText.h"
#include "GUI/BitmapFont.h"

#include "Logger.h"

const char FIRST_CHAR = ' ';
const char LAST_CHAR = '~';

const int NUM_GLYPHS_U = 16;
const int NUM_GLYPHS_V = 6;

BitmapText::BitmapText() : m_vertices(sf::Quads), m_color(255, 255, 255) {}

BitmapText::BitmapText(const sf::String &string, const BitmapFont &font) : m_font(&font), m_vertices(sf::Quads), m_string(string), m_color(255, 255, 255)
{
	init();
}

void BitmapText::setString(const sf::String &string)
{
	m_string = string;
	init();
}

const sf::String &BitmapText::getString() const
{
	return m_string;
}

void BitmapText::setFont(const BitmapFont &font)
{
	m_font = &font;
	init();
}

const BitmapFont *BitmapText::getFont() const
{
	return m_font;
}

void BitmapText::setColor(const sf::Color &color)
{
	m_color = color;
	init();	// TODO: could only replace vertex color attributes instead of all vertex data
}

const sf::Color &BitmapText::getColor() const
{
	return m_color;
}

void BitmapText::setCharacterSize(int size)
{
	m_characterSize = size;
	init();
}

const int BitmapText::getCharacterSize() const
{
	return m_characterSize;
}

sf::FloatRect BitmapText::getLocalBounds() const
{
	return m_bounds;
}

void BitmapText::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_font->getTexture();
	target.draw(m_vertices, states);
}

void BitmapText::init()
{
	m_vertices.clear();

	m_vertices.append(sf::Vertex(sf::Vector2f(-1000, -1000), sf::Color::White));
	m_vertices.append(sf::Vertex(sf::Vector2f(1000, -1000), sf::Color::White));
	m_vertices.append(sf::Vertex(sf::Vector2f(1000, 1000), sf::Color::White));
	m_vertices.append(sf::Vertex(sf::Vector2f(-1000, 1000), sf::Color::White));

	/*

	sf::Vector2i glyphSize = m_font->getGlyphSize();

	float dy = static_cast<float>(m_characterSize);
	float dx = glyphSize.x * (dy / glyphSize.y);

	float du = 1.f / NUM_GLYPHS_U;
	float dv = 1.f / NUM_GLYPHS_V;

	float curX = 0.f;
	float curY = 0.f;

	for (auto c : m_string)
	{
		if (c < FIRST_CHAR || c > LAST_CHAR)
		{
			c = '?';
		}

		if (c == '\t')
		{
			curX += 4 * dx;
		}
		else if (c == '\n')
		{
			curY += dy;
			curX = 0.f;
		}

		c -= FIRST_CHAR;

		float u = (c % NUM_GLYPHS_U) * du;
		float v = (c / NUM_GLYPHS_U) * dv;

		//m_vertices.append(sf::Vertex(sf::Vector2f(curX, curY), sf::Color::White, sf::Vector2f(u, v + dv)));
		//m_vertices.append(sf::Vertex(sf::Vector2f(curX + dx, curY), sf::Color::White, sf::Vector2f(u + du, v + dv)));
		//m_vertices.append(sf::Vertex(sf::Vector2f(curX + dx, curY + dy), sf::Color::White, sf::Vector2f(u + du, v)));
		//m_vertices.append(sf::Vertex(sf::Vector2f(curX, curY + dy), sf::Color::White, sf::Vector2f(u, v)));

		curX += dx;
	}

	m_bounds = m_vertices.getBounds();

	*/
}