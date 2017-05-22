#pragma once

#include "global.h"

// a simple float triangle, used for intersections
class FloatTriangle final {
public:
	FloatTriangle(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3);

	bool intersects(const sf::FloatRect& rect) const;
	bool contains(const sf::Vector2f& point) const;

private:
	sf::Vector2f m_vertex1;
	sf::Vector2f m_vertex2;
	sf::Vector2f m_vertex3;

	sf::FloatRect m_aabb;

private:
	// returns whether a segment between p1 and p2 interesects the rectangle rect.
	bool segmentRectangleIntersect(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::FloatRect& rect) const;
};