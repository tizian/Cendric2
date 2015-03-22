#include "stdafx.h"

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

void Screen::render(sf::RenderTarget &renderTarget) const
{
	// TODO: If everything on the screen is an object, something like this would work...

	//for (Object *obj : m_objects)
	//{
	//	obj->render(renderTarget);
	//}
}

void Screen::onEnter(Screen *previousScreen)
{
	cout << "Enter" << endl;
}

void Screen::onExit(Screen *nextScreen)
{
	cout << "Exit" << endl;
}

void Screen::addObject(Object *object)
{
	object->setScreen(this);
	m_objects.push_back(object);
}