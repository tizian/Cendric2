#pragma once

#include "global.h"
#include "ScreenManager.h"
#include "Screens\SplashScreen.h"
#include "InputController.h"

class Game
{
public:
	Game();

	void run();

private:
	bool m_running;
	sf::RenderWindow m_mainWindow;
	ScreenManager m_screenManager;
};