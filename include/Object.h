#pragma once

#include "stdafx.h"

// an abstract object with a gamestate and a bounding box
class Object
{
public:
	Object();
	virtual ~Object();

	virtual void update(sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget) const;

	void setScreen(Screen *screen);
	void setBoundingBox(sf::IntRect& rect);
	virtual void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPosition();
	sf::IntRect* getBoundingBox();

private:
	Screen *m_screen;
	IntRect m_boundingBox;
	// absolute position as seen from the upper left corner
	sf::Vector2f m_position;
};