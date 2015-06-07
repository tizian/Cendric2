#pragma once

#include <SFML/Graphics.hpp>

#include "BitmapFont.h"

class BitmapText : public sf::Drawable, public::sf::Transformable
{
public:
	BitmapText();
	BitmapText(const sf::String &string, const BitmapFont &font);
	BitmapText(const sf::String &string);

	void setString(const sf::String &string);
	const sf::String &getString() const;

	void setFont(const BitmapFont &font);
	const BitmapFont *getFont() const;

	void setColor(const sf::Color &color);
	const sf::Color &getColor() const;

	void setCharacterSize(int size);
	const int getCharacterSize() const;

	void setLineSpacing(float spacing);
	const float getLineSpacing() const;

	sf::FloatRect getLocalBounds() const;

private:
	void init();	// Set vertexArray data
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::String			m_string;
	const BitmapFont   *m_font = nullptr;
	int					m_characterSize;
	float				m_lineSpacing;
	sf::Color			m_color;
	sf::VertexArray		m_vertices;
	sf::FloatRect		m_bounds;
};