#include "GUI/BitmapText.h"
#include "GUI/BitmapFont.h"

const char FIRST_CHAR = ' ';
const char LAST_CHAR = 'ÿ';

const int NUM_GLYPHS_U = 16;
const int NUM_GLYPHS_V = 14;

void toUpperCase(sf::String &str)
{
	for (auto & c : str) {
		if (c == 228) c = 196;			// ä
		else if (c == 246) c = 214;		// ö
		else if (c == 252) c = 220;		// ü
		else c = toupper(c);
	}
}

BitmapText::BitmapText()
{
	m_vertices = sf::VertexArray(sf::Quads);
	m_color = sf::Color::White;
}

BitmapText::BitmapText(const sf::String &string, const BitmapFont &font)
{
	m_font = &font;
	m_vertices = sf::VertexArray(sf::Quads);
	m_string = string;
	toUpperCase(m_string);
	m_color = sf::Color::White;
	m_characterSize = font.getGlyphSize().y;
	init();
}

void BitmapText::setString(const sf::String &string)
{
	m_string = string;
	toUpperCase(m_string);
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
	using std::cout;
	using std::endl;

	m_vertices.clear();

	sf::Vector2i glyphSize = m_font->getGlyphSize();

	float dy = static_cast<float>(m_characterSize);
	float dx = glyphSize.x * (dy / glyphSize.y);

	float du = 1.f / NUM_GLYPHS_U * m_font->getTexture().getSize().x;
	float dv = 1.f / NUM_GLYPHS_V * m_font->getTexture().getSize().y;

	float curX = 0.f;
	float curY = 0.f;
    
//    sf::String blub(L"€ … †");
    
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

		m_vertices.append(sf::Vertex(sf::Vector2f(curX, curY), m_color, sf::Vector2f(u, v)));
		m_vertices.append(sf::Vertex(sf::Vector2f(curX + dx, curY), m_color, sf::Vector2f(u + du, v)));
		m_vertices.append(sf::Vertex(sf::Vector2f(curX + dx, curY + dy), m_color, sf::Vector2f(u + du, v + dv)));
		m_vertices.append(sf::Vertex(sf::Vector2f(curX, curY + dy), m_color, sf::Vector2f(u, v + dv)));

		curX += dx;
	}

	m_bounds = m_vertices.getBounds();
}