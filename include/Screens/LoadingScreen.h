#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Screens/MapScreen.h"
#include "Screens/GameScreen.h"

class LoadingScreen : public Screen
{
public:
	LoadingScreen(ScreenID screenToLoad, ResourceID level);

	Screen* update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

	virtual void execOnEnter(const Screen *previousScreen) override;
	virtual void execOnExit(const Screen *nextScreen) override;

private:
	sf::Sprite m_screenSprite;
	ScreenID m_screenToLoad;
	// depicts the map or level to load
	ResourceID m_resource;
};