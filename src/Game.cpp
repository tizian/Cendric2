#include "stdafx.h"

Game::Game()
{
	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cendric");
	m_mainWindow.setFramerateLimit(MAX_FRAME_RATE);
	m_screenManager = ScreenManager(new SplashScreen());
	m_running = true;
}

void Game::run()
{
	sf::Clock frameClock;
	sf::Time frameTime;
	frameTime = frameClock.restart();
	while (m_running)
	{
		
		sf::Event e;
		sf::Time deltaTime;
		
		// don't count this loop into the frametime!
		deltaTime = frameClock.restart();
		while (m_mainWindow.pollEvent(e))
		{
			if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
			{
				m_running = false;
			}
		}
		
		// time
		frameClock.restart();
		frameTime = deltaTime;

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