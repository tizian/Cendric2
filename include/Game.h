#pragma once

#include <list>

#include "Screens/ScreenManager.h"
#include "Screens/SplashScreen.h"

class Game final {
public:
	Game();
	~Game();

	void run();

private:
	bool m_running;
	sf::RenderWindow m_mainWindow;
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_mainSprite;
	ScreenManager* m_screenManager = nullptr;

	void reloadWindow();

	// debug operations
	std::list<float> m_fpsList;
	const int FPS_AVERAGE_NR = 10;
	void showFPSText(sf::RenderTarget& target, float frameTimeSeconds);
};