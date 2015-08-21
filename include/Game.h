#pragma once

#include <list>

#include "global.h"
#include "ScreenManager.h"
#include "Screens/SplashScreen.h"
#include "InputController.h"

#include "CharacterCore.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	bool m_running;
	sf::RenderWindow m_mainWindow;
	ScreenManager* m_screenManager = nullptr;

	// debug operations
	std::list<float> m_fpsList;
	const int FPS_AVERAGE_NR = 10;
	void showFPSText(sf::RenderTarget& target, float frameTimeSeconds);
};