#pragma once

#include "global.h"
#include "Screen.h"
#include "Screens\SplashScreen.h"
#include "Screens\MenuScreen.h"
#include "Screens\GameScreen.h"

class ScreenManager
{
public:
	ScreenManager();
	ScreenManager(Screen *initialScreen);

	void update(sf::Time frameTime);
	void render(sf::RenderTarget &renderTarget);

	Screen *getCurrentScreen();

private:
	Screen *m_currentScreen = nullptr;
};