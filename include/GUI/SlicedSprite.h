#pragma once

#include <SFML/Graphics.hpp>

#include "BitmapFont.h"

class SlicedSprite : public sf::Drawable, public::sf::Transformable
{
public:
	SlicedSprite();
	SlicedSprite(sf::Texture *tex, const sf::Color &color, int width, int height);

	void setSize(int width, int height);
	void setTexture(sf::Texture *texture);
	void setColor(sf::Color &color);

	void setSlicing(int left, int right, int top, int bottom);

	sf::FloatRect getLocalBounds() const;

private:
	void init();	// Set vertexArray data
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Texture *m_texture = nullptr;
	
	int m_width;
	int m_height;
	
	int m_leftSlice;
	int m_rightSlice;
	int m_topSlice;
	int m_bottomSlice;

	sf::Color			m_color;
	sf::VertexArray		m_vertices;
	sf::FloatRect		m_bounds;
};