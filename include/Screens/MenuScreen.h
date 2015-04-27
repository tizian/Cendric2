#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Screens\LoadingScreen.h"
#include "Screens\SplashScreen.h"
#include "Misc\FireBasket.h"

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