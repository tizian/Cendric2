#include "global.h"
#include "Game.h"
#include "DatabaseManager.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Logger.h"
#include "TextProvider.h"

// If this is defined, savegame and screenshot folders will be created outside of the Cendric directory at locations determined by the operating system. (e.g. User/Documents/Cendric on Windows)
//#define EXTERN_DOCUMENTS_FOLDER

#ifdef _WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "Platform/CendricWin32.h"
#elif __APPLE__
#include "Platform/CendricApple.h"
#endif

std::string g_documentsPath;

int main(int argc, char* argv[]) {

// show console window in windows only when debug mode is enabled.
#ifndef DEBUG
#ifdef _WIN32
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif
#endif

#ifdef EXTERN_DOCUMENTS_FOLDER
	g_documentsPath = getDocumentPath();
#else
	g_documentsPath = "";
#endif

	g_logger = new Logger();
	g_databaseManager = new DatabaseManager();
	g_resourceManager = new ResourceManager();
	g_textProvider = new TextProvider();
	g_inputController = new InputController();

	Game* game = new Game();
	game->run();
	delete game;

	delete g_resourceManager;
	delete g_inputController;
	delete g_textProvider;
	delete g_databaseManager;
	delete g_logger;

	return 0;
}