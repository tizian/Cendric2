#include "stdafx.h"

Object::Object() {}
Object::~Object() {}

void Object::update(float dt) {}
void Object::render(sf::RenderTarget &renderTarget) const {}

void Object::setScreen(Screen *screen)
{
	m_screen = screen;
}