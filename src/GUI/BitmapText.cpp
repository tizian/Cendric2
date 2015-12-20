#include "GUI/BitmapText.h"
#include "GUI/BitmapFont.h"
#include "ResourceManager.h"
#include <iterator>

const char FIRST_CHAR = ' ';

const int NUM_GLYPHS_U = 16;
const int NUM_GLYPHS_V = 14;

void toUpperCase(std::wstring& str) {
	for (auto& c : str) {
		if (c == 65508) c = L'Ä';			// ä
		else if (c == 65526) c = L'Ö';		// ö
		else if (c == 65532) c = L'Ü';		// ü
		else c = toupper(c);
	}
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
	m_string = std::wstring(string.begin(), string.end());
	toUpperCase(m_string);
	m_color = sf::Color::White;
	m_characterSize = font.getGlyphSize().y;
	m_lineSpacing = 0.5f;
	init();
}

BitmapText::BitmapText(const std::string& string) {
	m_font = g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default);
	m_vertices = sf::VertexArray(sf::Quads);
	m_string = std::wstring(string.begin(), string.end());
	toUpperCase(m_string);
	m_color = sf::Color::White;
	m_characterSize = m_font->getGlyphSize().y;
	m_lineSpacing = 0.5f;
	init();
}

void BitmapText::setString(const std::string& string) {
	m_string = std::wstring(string.begin(), string.end());
	toUpperCase(m_string);
	init();
}

const std::wstring &BitmapText::getString() const {
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

	for (auto c : m_string) {
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