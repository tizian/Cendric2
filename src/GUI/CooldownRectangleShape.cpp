#include "GUI/CooldownRectangleShape.h"
#include "Logger.h"

CooldownRectangleShape::CooldownRectangleShape() {
	m_width = 0.f;
	m_height = 0.f;
	m_angle = 0.f;
	m_triangleCount = 100;

	m_color = COLOR_WHITE;
	m_vertices = sf::VertexArray(sf::Triangles);

	init();
}

void CooldownRectangleShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = m_texture;

	int angleOffset = static_cast<int>(m_triangleCount / 360.f * m_angle);
	int numTrianglesToDraw = m_triangleCount - angleOffset;

	if (angleOffset == m_triangleCount) angleOffset = 0;
	const sf::Vertex *ver = &m_vertices[3*angleOffset];

	target.draw(ver, 3 * numTrianglesToDraw, sf::Triangles, states);
}

void CooldownRectangleShape::init() {
	m_vertices.clear();

	sf::Vector2f bo;
	sf::Vector2f to;
	sf::Vector2f bp1, bp2;
	sf::Vector2f tp1, tp2;

	bo = sf::Vector2f(sf::Vector2f(m_width / 2.f, m_height / 2.f));
	if (m_texture) {
		to = sf::Vector2f(m_texture->getSize().x / 2.f, m_texture->getSize().y / 2.f);
	}

	HelperRectangle bbox;
	HelperRectangle tbox;
	HelperRay bray(bo, sf::Vector2f(0.f, 0.f));
	HelperRay tray(to, sf::Vector2f(0.f, 0.f));

	bbox = HelperRectangle(sf::Vector2f(0.f, m_height), sf::Vector2f(m_width, 0.f));
	if (m_texture) {
		tbox = HelperRectangle(sf::Vector2f(0.f, (float)m_texture->getSize().y), sf::Vector2f((float)m_texture->getSize().x, 0.f));
	}

	float nearT, farT;
	float d = M_TWOPI / m_triangleCount;
	float theta = -M_PI / 2.f;

	bray.d = sf::Vector2f(std::cos(theta), std::sin(theta));
	tray.d = bray.d;
	
	bbox.rayIntersect(bray, nearT, farT);
	bp1 = bray(farT);

	if (m_texture) {
		tbox.rayIntersect(tray, nearT, farT);
		tp1 = tray(farT);
	}

	for (int i = 0; i < m_triangleCount; ++i) {
		theta += d;

		bray.d = sf::Vector2f(std::cos(theta), std::sin(theta));
		tray.d = bray.d;

		bbox.rayIntersect(bray, nearT, farT);
		bp2 = bray(farT);

		if (m_texture) {
			tbox.rayIntersect(tray, nearT, farT);
			tp2 = tray(farT);
		}
		
		m_vertices.append(sf::Vertex(bo, m_color, to));
		m_vertices.append(sf::Vertex(bp1, m_color, tp1));
		m_vertices.append(sf::Vertex(bp2, m_color, tp2));

		bp1 = bp2;
		tp1 = tp2;
	}
}

void CooldownRectangleShape::setSize(float width, float height) {
	m_width = width;
	m_height = height;
	init();
}

void CooldownRectangleShape::setAngle(float angle) {
	m_angle = angle;
}

void CooldownRectangleShape::setTexture(sf::Texture *texture) {
	m_texture = texture;
	init();
}

void CooldownRectangleShape::setFillColor(const sf::Color& color) {
	m_color = color;
	for (unsigned int i = 0; i < m_vertices.getVertexCount(); ++i) {
		m_vertices[i].color = m_color;
	}
}

float CooldownRectangleShape::getAngle() const {
	return m_angle;
}

sf::Texture *CooldownRectangleShape::getTexture() const {
	return m_texture;
}

const sf::Color CooldownRectangleShape::getFillColor() const {
	return m_color;
}
