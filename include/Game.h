#include "stdafx.h"

// TODO these values should not be const, but loaded from an .ini file.
#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 750
#define FRAME_RATE 60

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