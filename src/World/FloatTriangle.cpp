#include "World/FloatTriangle.h"

FloatTriangle::FloatTriangle(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3) {
	m_vertex1 = v1;
	m_vertex2 = v2;
	m_vertex3 = v3;

	// calculate aabb
	m_aabb.position.x = std::min(std::min(v1.x, v2.x), v3.x);
	m_aabb.position.y = std::min(std::min(v1.y, v2.y), v3.y);
	m_aabb.size.x = std::max(std::max(v1.x, v2.x), v3.x) - m_aabb.position.x;
	m_aabb.size.y = std::max(std::max(v1.y, v2.y), v3.y) - m_aabb.position.y;
}

bool FloatTriangle::intersects(const sf::FloatRect& rect) const {
	if (!fastIntersect(m_aabb, rect)) return false;

	sf::Vector2f rectCenter(rect.position.x + 0.5f * rect.size.x, rect.position.y + 0.5f * rect.size.y);
	if (contains(rectCenter)) return true;

	return
		segmentRectangleIntersect(m_vertex1, m_vertex2, rect) ||
		segmentRectangleIntersect(m_vertex2, m_vertex3, rect) ||
		segmentRectangleIntersect(m_vertex3, m_vertex1, rect);
}

inline float sign(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool FloatTriangle::contains(const sf::Vector2f& point) const {
	bool b1, b2, b3;

	b1 = sign(point, m_vertex1, m_vertex2) < 0.f;
	b2 = sign(point, m_vertex2, m_vertex3) < 0.f;
	b3 = sign(point, m_vertex3, m_vertex1) < 0.f;

	return ((b1 == b2) && (b2 == b3));
}

bool FloatTriangle::segmentRectangleIntersect(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::FloatRect& rect) const {
	float right = rect.position.x + rect.size.x;
	float bottom = rect.position.y + rect.size.y;

	// Completely outside.
	if ((p1.x <= rect.position.x && p2.x <= rect.position.x) 
		|| (p1.y <= rect.position.y && p2.y <= rect.position.y) 
		|| (p1.x >= right && p2.x >= right) 
		|| (p1.y >= bottom && p2.y >= bottom))
		return false;

	// that's a point, no segment. a quite flat triangle, that will never intersect.
	if (std::abs(p2.x - p1.x) < Epsilon) {
		return false;
	}

	float m = (p2.y - p1.y) / (p2.x - p1.x);

	float y = m * (rect.position.x - p1.x) + p1.y;
	if (y > rect.position.y && y < bottom) return true;

	y = m * (right - p1.x) + p1.y;
	if (y > rect.position.y && y < bottom) return true;

	float x = (rect.position.y - p1.y) / m + p1.x;
	if (x > rect.position.x && x < right) return true;

	x = (bottom - p1.y) / m + p1.x;
	if (x > rect.position.x && x < right) return true;

	return false;
}
