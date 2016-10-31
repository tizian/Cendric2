#pragma once

#include <SFML/Graphics.hpp>

#include "BitmapFont.h"

enum class TextAlignment {
	Left,
	Center,
	Right
};

inline static TextAlignment resolveTextAlignment(const std::string& alignment) {
	if (alignment.compare("left") == 0) return TextAlignment::Left;
	if (alignment.compare("right") == 0) return TextAlignment::Right;
	if (alignment.compare("center") == 0) return TextAlignment::Center;
	return TextAlignment::Left;
}

enum class TextStyle {
	Default,
	Shadowed
};

class BitmapText : public sf::Drawable, public::sf::Transformable {
public:
	BitmapText();
	BitmapText(const std::string& string, TextStyle style, TextAlignment alignment = TextAlignment::Left);
	BitmapText(const std::string& string, TextAlignment alignment = TextAlignment::Left);

	void setString(const std::string& string);
	const std::string& getString() const;

	void setFont(const BitmapFont& font);
	const BitmapFont* getFont() const;

	void setColor(const sf::Color& color);
	void setColorAlpha(sf::Uint8 alpha);
	const sf::Color& getColor() const;

	void setCharacterSize(int size);
	const int getCharacterSize() const;

	void setLineSpacing(float spacing);
	const float getLineSpacing() const;

	void setTextAlignment(TextAlignment alignment);
	TextAlignment getTextAlignment() const;

	void setTextStyle(TextStyle style);
	TextStyle getTextStyle() const;

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getBounds() const;

	// transforms the special characters of a string to characters that can be used in the bitmap text
	static std::string transform(const std::string& in);

private:
	void init();	// Set vertexArray data
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	static BitmapFont* getFont(TextStyle style, int characterSize);

private:
	std::string			m_string;
	const BitmapFont*   m_font = nullptr;
	int					m_characterSize;
	float				m_lineSpacing;
	sf::Color			m_color;
	sf::VertexArray		m_vertices;
	sf::FloatRect		m_bounds;
	TextAlignment		m_alignment;
	TextStyle			m_style;
};