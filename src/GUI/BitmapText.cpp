#include "GUI/BitmapText.h"
#include "GUI/BitmapFont.h"
#include "ResourceManager.h"
#include <iterator>

const char FIRST_CHAR = ' ';

const int NUM_GLYPHS_U = 16;
const int NUM_GLYPHS_V = 14;

std::string transform(const std::string& str) {
	std::string out;
	for (size_t i = 0; i < str.length(); ++i) {
		unsigned char c = str.at(i);
		if (c == 0xc3) {
			unsigned char c2 = str.at(i + 1);
			if (c2 == 0xa4u) {			// ä
				out.push_back(0xe4u);
			}
			else if (c2 == 0x84u) {		// Ä
				out.push_back(0xc4u);
			}
			else if (c2 == 0xb6u) {	// ö
				out.push_back(0xf6u);
			}
			else if (c2 == 0x96u) {	// Ö
				out.push_back(0xd6u);
			}
			else if (c2 == 0xbcu) {	// ü
				out.push_back(0xfcu);
			}
			else if (c2 == 0x9cu) {	// Ü
				out.push_back(0xdcu);
			}
			else {									// ?
				out.push_back(0x3fu);
			}
			i++;
		}
		else {
			//c = toupper(c);
			out.push_back(c);
		}
	}
	return out;
}

BitmapText::BitmapText() {
	m_vertices = sf::VertexArray(sf::Quads);
	m_color = sf::Color::White;
	m_font = g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default);
	m_characterSize = m_font->getGlyphSize().y;
	m_lineSpacing = 0.5f;
}

BitmapText::BitmapText(const std::string& string, const BitmapFont &font) {
	m_font = &font;
	m_vertices = sf::VertexArray(sf::Quads);
	m_string = transform(string);
	m_color = sf::Color::White;
	m_characterSize = font.getGlyphSize().y;
	m_lineSpacing = 0.5f;
	init();
}

BitmapText::BitmapText(const std::string& string) {
	m_font = g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default);
	m_vertices = sf::VertexArray(sf::Quads);
	m_string = transform(string);
	m_color = sf::Color::White;
	m_characterSize = m_font->getGlyphSize().y;
	m_lineSpacing = 0.5f;
	init();
}

void BitmapText::setString(const std::string& string) {
	m_string = transform(string);
	init();
}

const std::string& BitmapText::getString() const {
	return m_string;
}

void BitmapText::setFont(const BitmapFont& font) {
	m_font = &font;
	init();
}

const BitmapFont *BitmapText::getFont() const {
	return m_font;
}

void BitmapText::setColor(const sf::Color &color) {
	m_color = color;
	init();	// TODO: could only replace vertex color attributes instead of all vertex data
}

const sf::Color &BitmapText::getColor() const {
	return m_color;
}

void BitmapText::setCharacterSize(int size) {
	if (m_characterSize % m_font->getGlyphSize().y != 0) {
		// g_logger->logWarning("BitmapText::setCharacterSize", "You should only use multiples of the bitmap glyph size to avoid aliasing problems!");
	}
	m_characterSize = size;
	init();
}

const int BitmapText::getCharacterSize() const {
	return m_characterSize;
}

void BitmapText::setLineSpacing(float spacing) {
	m_lineSpacing = spacing;
	init();
}

const float BitmapText::getLineSpacing() const {
	return m_lineSpacing;
}

sf::FloatRect BitmapText::getLocalBounds() const {
	return m_bounds;
}

sf::FloatRect BitmapText::getBounds() const {
	return getTransform().transformRect(m_bounds);
}

void BitmapText::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &m_font->getTexture();
	target.draw(m_vertices, states);
}

void BitmapText::init() {
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

	for (size_t i = 0; i < m_string.length(); ++i) {
		unsigned char c = m_string.at(i);
		if (c == '\t') {
			curX += 4 * dx;
			continue;
		}
		else if (c == '\n') {
			curY += dy * (m_lineSpacing + 1.f);
			curX = 0.f;
			continue;
		}

		if (c < FIRST_CHAR) {
			c = '?';
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