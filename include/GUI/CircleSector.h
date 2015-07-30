#pragma once

#include <SFML/Graphics.hpp>

class CircleSector : public sf::Drawable, public::sf::Transformable
{
public:
	CircleSector();
	CircleSector(float radius);

	void setAngle(float angle);
	void setRadius(float radius);
	void setPointCount(int count);
	void setTexture(sf::Texture *texture);
	void setFillColor(const sf::Color &color);

	float getAngle() const;
	float getRadius() const;
	int getPointCount() const;
	sf::Texture *getTexture() const;
	const sf::Color getFillColor() const;

	sf::FloatRect getLocalBounds() const { return sf::FloatRect(0.f, 0.f, 2.f * m_radius, 2.f * m_radius); }

private:
	void init();	// Set vertexArray data
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Texture *m_texture = nullptr;
	
	float m_radius;
	float m_angle;
	
	int m_pointCount;

	sf::Color			m_color;
	sf::VertexArray		m_vertices;
};