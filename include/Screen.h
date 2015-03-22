#pragma once

#include "stdafx.h"

class Object;
class ScreenManager;

class Screen
{
public:
	Screen();
	virtual ~Screen();

	virtual void update(float dt);
	virtual void render(sf::RenderTarget &renderTarget) const;

	virtual void onEnter(Screen *previousScreen);
	virtual void onExit(Screen *nextScreen);

	void addObject(Object *object);

	void setScreenManager(ScreenManager *manager);
	
protected:
	ScreenManager *m_screenManager;
	vector<Object *> m_objects;
};