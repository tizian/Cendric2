#pragma once

#include <SFML/Graphics.hpp>

#include "BitmapFont.h"

class SlicedSprite final : public sf::Drawable, public sf::Transformable {
public:
	SlicedSprite();
	SlicedSprite(sf::Texture* tex, const sf::Color& color, float width, float height);

	void setSize(const sf::Vector2f& size);
	void setSize(float width, float height);
	void setTexture(sf::Texture *texture);
	void setColor(const sf::Color& color);

	sf::Vector2f getSize() const;

	sf::Color getColor() const;

	void setSlicing(float left, float right, float top, float bottom);

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getBounds() const;

private:
	void init();	// Set vertexArray data
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Texture* m_texture = nullptr;

	float m_width;
	float m_height;

	float m_leftSlice;
	float m_rightSlice;
	float m_topSlice;
	float m_bottomSlice;

	sf::Color			m_color;
	sf::VertexArray		m_vertices;
	sf::FloatRect		m_bounds;
};