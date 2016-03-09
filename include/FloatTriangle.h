#pragma once

#include "global.h"

// a simple float triangle, used for intersections
class FloatTriangle {
public:
	FloatTriangle(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3);
	~FloatTriangle() {};

	bool intersects(const sf::FloatRect& rect) const;

private:
	sf::Vector2f m_vertex1;
	sf::Vector2f m_vertex2;
	sf::Vector2f m_vertex3;

	sf::FloatRect m_aabb;

private:
	// returns whether a line between p1 and p2 interesects the rectangle rect.
	bool lineRectangleIntersect(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::FloatRect& rect) const;
};