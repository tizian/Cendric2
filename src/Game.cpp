#include "stdafx.h"

void Game::init()
{
	m_mainWindow.create(sf::VideoMode(__WINDOW_WIDTH, __WINDOW_HEIGHT), "Cendric");

	m_currentLevel.loadFromFile("res/level/testlevel.dric");
	m_controller.init();
	m_gameState = GameState::ShowingScreenSplash;
}

void Game::start(void)
{
	if (m_gameState != GameState::Uninitialized)
		return;

	init();

	while (!isExiting())
	{
		gameLoop();
	}

	m_mainWindow.close();
}

bool Game::isExiting()
{
	return (m_gameState == GameState::Exiting);
}

void Game::gameLoop()
{
	sf::Event currentEvent;
	while (m_mainWindow.pollEvent(currentEvent))
	{
		switch (m_gameState)
		{
		case GameState::ShowingScreenMenu:
		{
			showMenuScreen();
			break;
		}
		case GameState::ShowingScreenSplash:
		{
			showSplashScreen();
			break;
		}
		case GameState::PlayingLevel:
		{
			m_controller.update();

			m_mainWindow.clear();
			m_mainWindow.draw(m_currentLevel.getTilemap());
			m_mainWindow.display();

			if (currentEvent.type == sf::Event::Closed || (currentEvent.type == sf::Event::KeyPressed && currentEvent.key.code == sf::Keyboard::Escape))
			{
				m_gameState = GameState::Exiting;
			}
			break;
		}
		}
	}
}

void Game::showSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.show(m_mainWindow);
	m_gameState = GameState::ShowingScreenMenu;
}

void Game::showMenuScreen()
{
	MenuScreen mainMenu;
	MenuScreen::MenuResult result = mainMenu.show(m_mainWindow);
	switch (result)
	{
	case MenuScreen::MenuResult::Exit:
	{
		m_gameState = GameState::Exiting;
		break;
	}
	case MenuScreen::MenuResult::Play:
	{
		m_gameState = GameState::PlayingLevel;
		break;
	}
	}
}


Game::GameState Game::m_gameState = GameState::Uninitialized;
sf::RenderWindow Game::m_mainWindow;
Level Game::m_currentLevel;
InputController Game::m_controller;