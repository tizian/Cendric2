#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "MapScreen.h"
#include "Screens/GameScreen.h"

class LoadingScreen : public Screen
{
public:
	LoadingScreen(ScreenID screenToLoad, ResourceID level);

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

	virtual void onEnter(Screen *previousScreen);
	virtual void onExit(Screen *nextScreen);

private:
	sf::Sprite m_screenSprite;
	ScreenID m_screenToLoad;
	// depicts the map or level to load
	ResourceID m_resource;
	bool m_isLoading;
};