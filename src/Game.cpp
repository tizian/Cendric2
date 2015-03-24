#include "stdafx.h"

Game::Game()
{
	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cendric");
	m_mainWindow.setFramerateLimit(FRAME_RATE);
	m_screenManager = ScreenManager(new SplashScreen());
	m_running = true;
}

void Game::run()
{
	sf::Clock frameClock;

	while (m_running)
	{
		sf::Event e;
		while (m_mainWindow.pollEvent(e))
		{
			if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
			{
				m_running = false;
			}
		}

		// time
		sf::Time frameTime = frameClock.restart();

		// input
		g_inputController->update();

		// game updates
		m_screenManager.update(frameTime);

		// render
		m_mainWindow.clear();
		m_screenManager.render(m_mainWindow);
		m_mainWindow.display();
	}

	m_mainWindow.close();
}