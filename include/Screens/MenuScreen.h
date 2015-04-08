#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "LoadingScreen.h"
#include "SplashScreen.h"

class MenuScreen : public Screen
{
public:
	MenuScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

	void onEnter(Screen *previousScreen) override;
	void onExit(Screen *nextScreen) override;

private:
	sf::Sprite m_screenSprite;
};