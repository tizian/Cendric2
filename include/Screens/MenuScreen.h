#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "LoadingScreen.h"
#include "SplashScreen.h"
#include "FireBasket.h"

class MenuScreen : public Screen
{
public:
	MenuScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

	void execOnEnter(Screen *previousScreen) override;
	void execOnExit(Screen *nextScreen) override;

private:
	sf::Sprite m_screenSprite;
};