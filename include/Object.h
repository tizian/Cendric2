#pragma once

#include "global.h"

class Screen;

// an abstract object with a gamestate, a screen and a bounding box
class Object
{
public:
	Object();
	virtual ~Object();

	virtual void update(sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget) const;

	void setBoundingBox(const sf::FloatRect& rect);
	virtual void setPosition(const sf::Vector2f& pos);
	void setPositionX(float posX);
	void setPositionY(float posY);
	const sf::Vector2f& getPosition();
	sf::FloatRect* getBoundingBox();
	sf::Vector2f Object::getCenter();

private:
	sf::FloatRect m_boundingBox;
	// absolute position as seen from the upper left corner
	sf::Vector2f m_position;
};