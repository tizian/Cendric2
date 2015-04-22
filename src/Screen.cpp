#include "Screen.h"

using namespace std;

Screen::Screen()
{
	m_objects = vector<GameObject *>();
}

Screen::~Screen() {}

Screen* Screen::update(sf::Time frameTime)
{
	for (GameObject *obj : m_objects)
	{
		obj->update(frameTime);
	}
	return this;
}

void Screen::render(sf::RenderTarget &renderTarget)
{
	for (GameObject *obj : m_objects)
	{
		obj->render(renderTarget);
	}
}

void Screen::addObject(GameObject *object)
{
	m_objects.push_back(object);
}