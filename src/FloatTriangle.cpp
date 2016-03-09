#include "FloatTriangle.h"

FloatTriangle::FloatTriangle(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3) {
	m_vertex1 = v1;
	m_vertex2 = v2;
	m_vertex3 = v3;

	// calculate aabb
	m_aabb.left = std::min(std::min(v1.x, v2.x), v3.x);
	m_aabb.top = std::min(std::min(v1.y, v2.y), v3.y);
	m_aabb.width = std::max(std::max(v1.x, v2.x), v3.x) - m_aabb.left;
	m_aabb.height = std::max(std::max(v1.y, v2.y), v3.y) - m_aabb.top;
}

bool FloatTriangle::intersects(const sf::FloatRect& rect) const {
	if (!m_aabb.intersects(rect)) return false;

	return
		lineRectangleIntersect(m_vertex1, m_vertex2, rect) ||
		lineRectangleIntersect(m_vertex2, m_vertex3, rect) ||
		lineRectangleIntersect(m_vertex3, m_vertex1, rect);
}

bool FloatTriangle::lineRectangleIntersect(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::FloatRect& rect) const {
	float topIntersection;
	float bottomIntersection;
	float topTrianglePoint;
	float bottomTrianglePoint;

	// Calculate m and c for the equation for the line (y = mx+c)
	float m = (p2.y - p1.y) / (p2.x - p1.x);
	float c = p1.y - (m * p1.x);

	// if the line is going up from right to left then the top intersect point is on the left
	if (m > 0.f) {
		topIntersection = m * rect.left + c;
		bottomIntersection = m * (rect.left + rect.width) + c;
	}
	// otherwise it's on the right
	else {
		topIntersection = m * (rect.left + rect.width) + c;
		bottomIntersection = m * rect.left + c;
	}

	// work out the top and bottom extents for the triangle
	if (p1.y < p2.y) {
		topTrianglePoint = p1.y;
		bottomTrianglePoint = p2.y;
	}
	else {
		topTrianglePoint = p2.y;
		bottomTrianglePoint = p1.y;
	}

	// and calculate the overlap between those two bounds
	float topOverlap = topIntersection > topTrianglePoint ? topIntersection : topTrianglePoint;
	float bottomOverlap = bottomIntersection < bottomTrianglePoint ? bottomIntersection : bottomTrianglePoint;

	// (topoverlap<botoverlap) :
	// if the intersection isn't the right way up then we have no overlap

	// (!((botoverlap<t) || (topoverlap>b)) :
	// If the bottom overlap is higher than the top of the rectangle or the top overlap is
	// lower than the bottom of the rectangle we don't have intersection. So return the negative
	// of that. Much faster than checking each of the points is within the bounds of the rectangle.
	return (topOverlap < bottomOverlap) && (!((bottomOverlap < rect.top) || (topOverlap >(rect.top + rect.height))));
}