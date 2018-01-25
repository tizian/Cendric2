#pragma once

#include <SFML/Graphics.hpp>

struct HelperRay final {
	sf::Vector2f o, d;

	HelperRay() {}
	HelperRay(sf::Vector2f o, sf::Vector2f d) : o(o), d(d) {}
	sf::Vector2f operator()(float t) const {
		return o + t * d;
	}
};

struct HelperRectangle final {
	sf::Vector2f min, max;

	HelperRectangle() {}
	HelperRectangle(sf::Vector2f min, sf::Vector2f max) : min(min), max(max) {}
	bool rayIntersect(const HelperRay& ray, float& nearT, float& farT) {
		float t0 = 0.f, t1 = 1000.f;
		{
			float invRayDir = 1.f / ray.d.x;
			float tNear = (min.x - ray.o.x) * invRayDir;
			float tFar = (max.x - ray.o.x) * invRayDir;

			if (tNear > tFar) std::swap(tNear, tFar);

			t0 = tNear > t0 ? tNear : t0;
			t1 = tFar < t1 ? tFar : t1;
			if (t0 > t1) return false;
		}
		{
			float invRayDir = 1.f / ray.d.y;
			float tNear = (min.y - ray.o.y) * invRayDir;
			float tFar = (max.y - ray.o.y) * invRayDir;

			if (tNear > tFar) std::swap(tNear, tFar);

			t0 = tNear > t0 ? tNear : t0;
			t1 = tFar < t1 ? tFar : t1;
			if (t0 > t1) return false;
		}
		nearT = t0;
		farT = t1;
		return true;
	}
};

class CooldownRectangleShape final : public sf::Drawable, public sf::Transformable {
public:
	CooldownRectangleShape();

	void setSize(float width, float height);
	void setAngle(float angle);
	void setTexture(sf::Texture* texture);
	void setFillColor(const sf::Color& color);

	float getAngle() const;
	sf::Texture* getTexture() const;
	const sf::Color getFillColor() const;

	sf::FloatRect getLocalBounds() const { return { 0.f, 0.f, m_width, m_height }; }

private:
	void init();	// Set vertexArray data
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Texture *m_texture = nullptr;

	float m_width;
	float m_height;

	float m_angle;

	int	m_triangleCount;
	sf::Color			m_color;
	sf::VertexArray		m_vertices;
};