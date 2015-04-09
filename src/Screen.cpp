#include "Screen.h"

using namespace std;

Screen::Screen()
{
	m_objects = vector<Object *>();
}

Screen::~Screen() {}

Screen* Screen::update(sf::Time frameTime)
{
	for (Object *obj : m_objects)
	{
		obj->update(frameTime);
	}
	return this;
}

void Screen::render(sf::RenderTarget &renderTarget)
{
	for (Object *obj : m_objects)
	{
		obj->render(renderTarget);
	}
}

void Screen::addObject(Object *object)
{
	m_objects.push_back(object);
}