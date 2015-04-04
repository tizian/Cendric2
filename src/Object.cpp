#include "stdafx.h"

Object::Object() {}
Object::~Object() {}

void Object::update(sf::Time& frameTime) {}
void Object::render(sf::RenderTarget& renderTarget) const {}

void Object::setScreen(Screen *screen)
{
	m_screen = screen;
}

void Object::setBoundingBox(const sf::FloatRect& rect)
{
	m_boundingBox = rect;
}

sf::FloatRect* Object::getBoundingBox()
{
	return &m_boundingBox;
}

const Vector2f& Object::getPosition()
{
	return m_position;
}

void Object::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_boundingBox.left = pos.x;
	m_boundingBox.top = pos.y;
}

void Object::setPositionX(float posX)
{
	m_position.x = posX;
}

void Object::setPositionY(float posY)
{
	m_position.y = posY;
}