#include "GUI/SlicedSprite.h"
#include "Logger.h"

SlicedSprite::SlicedSprite()
{
	m_width = 0;
	m_height = 0;
	m_leftSlice = 0;
	m_rightSlice = 0;
	m_topSlice = 0;
	m_bottomSlice = 0;
	m_vertices = sf::VertexArray(sf::Quads);
	m_color = sf::Color::White;
}

SlicedSprite::SlicedSprite(sf::Texture *tex, const sf::Color &color, int width, int height)
{
	m_texture = tex;
	m_width = width;
	m_height = height;
	m_leftSlice = 0;
	m_rightSlice = 0;
	m_topSlice = 0;
	m_bottomSlice = 0;
	m_vertices = sf::VertexArray(sf::Quads);
	m_color = color;

	// For default behaviour, assume symmetric slicing
	int texWidth = tex->getSize().x;
	int texHeight = tex->getSize().y;
	m_leftSlice = std::floor(texWidth / 2);
	m_rightSlice = m_leftSlice;
	m_topSlice = std::floor(texHeight / 2);
	m_bottomSlice = m_topSlice;

	init();
}

void SlicedSprite::setSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void SlicedSprite::setTexture(sf::Texture *texture)
{
	m_texture = texture;
}

void SlicedSprite::setColor(sf::Color &color)
{
	m_color = color;

	for (int i = 0; i < m_vertices.getVertexCount(); ++i) {
		m_vertices[i].color = m_color;
	}
}

void SlicedSprite::setSlicing(int left, int right, int top, int bottom)
{
	m_leftSlice = left;
	m_rightSlice = right;
	m_topSlice = top;
	m_bottomSlice = bottom;
	init();
}

sf::FloatRect SlicedSprite::getLocalBounds() const
{
	return m_bounds;
}

void SlicedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = m_texture;
	target.draw(m_vertices, states);
}

void SlicedSprite::init()
{
	m_vertices.clear();

	if (m_leftSlice + m_rightSlice > m_width || m_topSlice + m_bottomSlice > m_height) {
		g_logger->logError("SlicedSprite::init()", "Slicing doesn't agree with width and height!");
		return;
	}

	int horizontal = m_width - m_leftSlice - m_rightSlice;
	int vertical = m_height - m_topSlice - m_bottomSlice;

	int texWidth = m_texture->getSize().x;
	int texHeight = m_texture->getSize().y;

	int x[4] = { 0, m_leftSlice, m_width - m_rightSlice, m_width };
	int y[4] = { 0, m_topSlice, m_height - m_bottomSlice, m_height };

	int u[4] = { 0, m_leftSlice, texWidth - m_rightSlice, texWidth };
	int v[4] = { 0, m_topSlice, texHeight - m_bottomSlice, texHeight };

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			m_vertices.append(sf::Vertex(sf::Vector2f(x[i], y[j]), m_color, sf::Vector2f(u[i], v[j])));
			m_vertices.append(sf::Vertex(sf::Vector2f(x[i + 1], y[j]), m_color, sf::Vector2f(u[i + 1], v[j])));
			m_vertices.append(sf::Vertex(sf::Vector2f(x[i + 1], y[j + 1]), m_color, sf::Vector2f(u[i + 1], v[j + 1])));
			m_vertices.append(sf::Vertex(sf::Vector2f(x[i], y[j + 1]), m_color, sf::Vector2f(u[i], v[j + 1])));
		}
	}

	m_bounds = m_vertices.getBounds();
}