#pragma once

#include <vector>

#include "global.h"
#include "Object.h"

class Object;
class ScreenManager;

enum class ScreenID
{
	Screen_map,
	Screen_game
};

class Screen
{
public:
	Screen();
	virtual ~Screen();

	virtual Screen* update(sf::Time frameTime);
	virtual void render(sf::RenderTarget &renderTarget);

	virtual void onEnter(Screen *previousScreen);
	virtual void onExit(Screen *nextScreen);

	void addObject(Object *object);
	
protected:
	std::vector<Object *> m_objects;
};