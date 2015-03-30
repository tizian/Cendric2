#include "stdafx.h"

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