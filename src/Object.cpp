#include "stdafx.h"

Object::Object() {}
Object::~Object() {}

void Object::update(sf::Time frameTime) {}
void Object::render(sf::RenderTarget &renderTarget) const {}

void Object::setScreen(Screen *screen)
{
	m_screen = screen;
}