#pragma once

#include <vector>

#include "global.h"
#include "GameObject.h"

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

	virtual void onEnter(Screen *previousScreen) = 0;
	virtual void onExit(Screen *nextScreen) = 0;

	void addObject(GameObject *object);
	
protected:
	std::vector<GameObject *> m_objects;
};