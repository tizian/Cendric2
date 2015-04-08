#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "MenuScreen.h"

class SplashScreen : public Screen
{
public:
	SplashScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override; 

	void onEnter(Screen *previousScreen) override;
	void onExit(Screen *nextScreen) override;

private:	
	sf::Sprite m_screenSprite;
};