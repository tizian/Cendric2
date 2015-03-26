#include "stdafx.h"

Object::Object() {}
Object::~Object() {}

void Object::update(sf::Time& frameTime) {}
void Object::render(sf::RenderTarget& renderTarget) const {}

void Object::setScreen(Screen *screen)
{
	m_screen = screen;
}

void Object::setBoundingBox(sf::IntRect& rect)
{
	m_boundingBox = rect;
}

sf::IntRect* Object::getBoundingBox()
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