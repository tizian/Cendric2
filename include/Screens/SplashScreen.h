#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "Misc\FireBasket.h"

class SplashScreen : public Screen
{
public:
	SplashScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override; 

	void execOnEnter(Screen *previousScreen) override;
	void execOnExit(Screen *nextScreen) override;

private:	
	sf::Sprite m_screenSprite;
};