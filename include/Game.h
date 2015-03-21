#include "stdafx.h"

// TODO these values should not be const, but loaded from an .ini file.
#define __WINDOW_WIDTH 1600
#define __WINDOW_HEIGHT 960

class Game
{

public:
	static void start();

private:
	static bool isExiting();
	static void gameLoop();
	static void init();

	static void showSplashScreen();
	static void showMenuScreen();

	enum class GameState {
		Uninitialized,
		ShowingScreenSplash,
		ShowingScreenMenu,
		Paused,
		PlayingMap,
		PlayingDialogue,
		PlayingLevel,
		Exiting
	};

	static GameState m_gameState;
	static sf::RenderWindow m_mainWindow;
	static Level m_currentLevel;
	static InputController m_controller;
};