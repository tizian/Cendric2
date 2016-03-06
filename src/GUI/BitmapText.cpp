#include "GUI/BitmapText.h"
#include "GUI/BitmapFont.h"
#include "ResourceManager.h"
#include <iterator>
#include <sstream>

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
	m_font = g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default_8);
	m_characterSize = m_font->getGlyphSize().y;
	m_lineSpacing = 0.5f;
	m_alignment = TextAlignment::Left;
}

BitmapText::BitmapText(const std::string& string, const BitmapFont& font, TextAlignment alignment) {
	m_font = &font;
	m_vertices = sf::VertexArray(sf::Quads);
	m_string = transform(string);
	m_color = sf::Color::White;
	m_characterSize = font.getGlyphSize().y;
	m_lineSpacing = 0.5f;
	m_alignment = alignment;
	init();
}

BitmapText::BitmapText(const std::string& string, TextAlignment alignment) {
	m_font = g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default_8);
	m_vertices = sf::VertexArray(sf::Quads);
	m_string = transform(string);
	m_color = sf::Color::White;
	m_characterSize = m_font->getGlyphSize().y;
	m_lineSpacing = 0.5f;
	m_alignment = alignment;
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

const BitmapFont* BitmapText::getFont() const {
	return m_font;
}

void BitmapText::setColor(const sf::Color& color) {
	m_color = color;
	init();	// TODO: could only replace vertex color attributes instead of all vertex data
}

const sf::Color& BitmapText::getColor() const {
	return m_color;
}

void BitmapText::setCharacterSize(int size) {
	if (size % 12 == 0) {
		m_font = g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default_12);
	}
	else {
		if (m_characterSize % 8 != 0) {
			g_logger->logWarning("BitmapText::setCharacterSize", "You should only use multiples of the bitmap glyph sizes (8 or 12) to avoid aliasing problems!");
		}
		m_font = g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default_8);
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

void BitmapText::setTextAlignment(TextAlignment alignment) {
	m_alignment = alignment;
	init();
}

const TextAlignment BitmapText::getTextAlignment() const {
	return m_alignment;
}

sf::FloatRect BitmapText::getLocalBounds() const {
	return m_bounds;
}

sf::FloatRect BitmapText::getBounds() const {
	return getTransform().transformRect(m_bounds);
}

void BitmapText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &m_font->getTexture();
	target.draw(m_vertices, states);
}

void BitmapText::init() {
	if (m_string == "") return;

	std::vector<std::string> lines;
	std::istringstream ss(m_string);
	std::string line;
	while (getline(ss, line, '\n')) {
		lines.push_back(line);
	}
	size_t maxLineLength = 0;
	for (size_t i = 0; i < lines.size(); ++i) {
		size_t size = lines[i].size();
		if (size > maxLineLength) {
			maxLineLength = size;
		}
	}

	m_vertices.clear();

	sf::Vector2i glyphSize = m_font->getGlyphSize();

	float dy = static_cast<float>(m_characterSize);
	float dx = glyphSize.x * (dy / glyphSize.y);

	float du = 1.f / NUM_GLYPHS_U * m_font->getTexture().getSize().x;
	float dv = 1.f / NUM_GLYPHS_V * m_font->getTexture().getSize().y;

	float curX = 0.f;
	float curY = 0.f;
	
	int lineNumber = 0;
	if (m_alignment == TextAlignment::Center) {
		curX = 0.5f * (lines[lineNumber].size() - maxLineLength) * dx;
	}
	else if (m_alignment == TextAlignment::Right) {
		curX = (lines[lineNumber].size() - maxLineLength) * dx;
	}
	
	for (size_t i = 0; i < m_string.length(); ++i) {
		unsigned char c = m_string.at(i);
		if (c == '\t') {
			curX += 4 * dx;
			continue;
		}
		else if (c == '\n') {
			curY += dy * (m_lineSpacing + 1.f);
			lineNumber++;
			curX = 0.f;
			if (lineNumber < lines.size()) {
				if (m_alignment == TextAlignment::Center) {
					curX = 0.5f * (maxLineLength - lines[lineNumber].size()) * dx;
				}
				else if (m_alignment == TextAlignment::Right) {
					curX = (maxLineLength - lines[lineNumber].size()) * dx;
				}
			}
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

	float width = dx * maxLineLength;
	float height = dy * (m_lineSpacing + 1.f) * lines.size() - m_lineSpacing * dy;
	m_bounds = sf::FloatRect(0.f, 0.f, width, height);
}