#pragma once

#include "global.h"

class Screen;

// an abstract object with a gamestate and a bounding box
class Object
{
public:
	Object();
	virtual ~Object();

	virtual void update(sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget) const;

	void setScreen(Screen *screen);
	void setBoundingBox(const sf::FloatRect& rect);
	virtual void setPosition(const sf::Vector2f& pos);
	void setPositionX(float posX);
	void setPositionY(float posY);
	const sf::Vector2f& getPosition();
	sf::FloatRect* getBoundingBox();

private:
	Screen *m_screen;
	sf::FloatRect m_boundingBox;
	// absolute position as seen from the upper left corner
	sf::Vector2f m_position;
};