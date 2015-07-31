#include "GUI/CircleSector.h"
#include "Logger.h"

CircleSector::CircleSector()
{
	m_radius = 0.f;
	m_angle = 360.f;

	m_pointCount = 100;
	m_color = sf::Color::White;
	m_vertices = sf::VertexArray(sf::Triangles);

	init();
}

CircleSector::CircleSector(float radius)
{
	m_radius = radius;
	m_angle = 360.f;

	m_pointCount = 100;
	m_color = sf::Color::White;
	m_vertices = sf::VertexArray(sf::Triangles);

	init();
}

void CircleSector::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = m_texture;
    const sf::Vertex *ver = &m_vertices[0];

    int numTriangles = static_cast<int>(m_pointCount / 360.f * m_angle);

    target.draw(ver, 3 * numTriangles, sf::Triangles, states);
}

void CircleSector::init()
{
	m_vertices.clear();

	float texRadius = 0.f;
	if (m_texture)
	{
		if (m_texture->getSize().x != m_texture->getSize().y)
		{
			g_logger->logError("CircleSector::init()", "Can only use square textures with CircleSector!");
			return;
		}
		texRadius = static_cast<float>(m_texture->getSize().y) / 2.f;
	}

	float d = M_TWOPI / m_pointCount;
	float theta = - M_PI / 2.f;

	float x0 = m_radius;
	float y0 = m_radius;

	float u0 = texRadius;
	float v0 = texRadius;

	for (int i = 0; i < m_pointCount; ++i)
	{
		float x1 = x0 + m_radius * std::cos(theta);
		float y1 = y0 + m_radius * std::sin(theta);

		float x2 = x0 + m_radius * std::cos(theta + d);
		float y2 = y0 + m_radius * std::sin(theta + d);

		float u1 = 0.f; float u2 = 0.f;
		float v1 = 0.f; float v2 = 0.f;

		if (m_texture) {
			u1 = u0 + texRadius * std::cos(theta);
			v1 = v0 + texRadius * std::sin(theta);

			u2 = u0 + texRadius * std::cos(theta + d);
			v2 = v0 + texRadius * std::sin(theta + d);
		}
		
		m_vertices.append(sf::Vertex(sf::Vector2f(x0, y0), m_color, sf::Vector2f(u0, v0)));
		m_vertices.append(sf::Vertex(sf::Vector2f(x1, y1), m_color, sf::Vector2f(u1, v1)));
		m_vertices.append(sf::Vertex(sf::Vector2f(x2, y2), m_color, sf::Vector2f(u2, v2)));

		theta += d;
	}
}

void CircleSector::setAngle(float angle)
{
	m_angle = angle;
}

void CircleSector::setRadius(float radius)
{
	m_radius = radius;
	init();
}

void CircleSector::setPointCount(int count)
{
	m_pointCount = count;
	init();
}

void CircleSector::setTexture(sf::Texture *texture)
{
	m_texture = texture;
	init();
}

void CircleSector::setFillColor(const sf::Color &color)
{
	m_color = color;
	for (unsigned int i = 0; i < m_vertices.getVertexCount(); ++i) {
		m_vertices[i].color = m_color;
	}
}

float CircleSector::getAngle() const
{
	return m_angle;
}

float CircleSector::getRadius() const
{
	return m_radius;
}

int CircleSector::getPointCount() const
{
	return m_pointCount;
}

sf::Texture *CircleSector::getTexture() const
{
	return m_texture;
}

const sf::Color CircleSector::getFillColor() const
{
	return m_color;
}
