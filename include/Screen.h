#pragma once

#include "stdafx.h"

class Object;
class ScreenManager;

class Screen
{
public:
	Screen();
	virtual ~Screen();

	virtual Screen* update(sf::Time frameTime);
	virtual void render(sf::RenderTarget &renderTarget) const;

	virtual void onEnter(Screen *previousScreen);
	virtual void onExit(Screen *nextScreen);

	void addObject(Object *object);
	
protected:
	vector<Object *> m_objects;
};