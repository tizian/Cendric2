#include "stdafx.h"

Game::Game()
{
	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cendric");
	m_screenManager = ScreenManager(new SplashScreen());
	m_inputController.init();

	m_running = true;
}

void Game::run()
{
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

		float dt = 0.0f; // TODO: frame time
		m_screenManager.update(dt);

		m_mainWindow.clear();

		m_screenManager.render(m_mainWindow);

		m_mainWindow.display();
	}

	m_mainWindow.close();
}