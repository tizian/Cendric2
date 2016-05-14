#include "global.h"
#include "Game.h"
#include "DatabaseManager.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Logger.h"
#include "TextProvider.h"

#ifdef _WIN32

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <ShlObj.h>
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

g_documentsPath = "";

// If this is defined, savegame and screenshot folders will be created outside of the Cendric directory at locations determined by the operating system. (e.g. User/Documents/Cendric on Windows)
// #define EXTERN_DOCUMENTS_FOLDER
#ifdef EXTERN_DOCUMENTS_FOLDER

#if _WIN32

bool success = false;
TCHAR path[MAX_PATH];
if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, path))) {
	char* folder = "\\My Games\\Cendric\\";
	strcat(path, folder);

	g_documentsPath = path;

	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, path, -1, wstr, wchars_num);
	if (SHCreateDirectory(NULL, wstr) == ERROR_SUCCESS) {
		{
			TCHAR* saves = "saves";
			TCHAR pathCopy[MAX_PATH];
			strcpy(pathCopy, path);
			strcat(pathCopy, saves);

			int wchars_num_copy = MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, NULL, 0);
			wchar_t* wstr_copy = new wchar_t[wchars_num_copy];
			MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, wstr_copy, wchars_num_copy);
			SHCreateDirectory(NULL, wstr_copy);
			delete[] wstr_copy;
		}
		{
			TCHAR* screenshots = "screenshots";
			TCHAR pathCopy[MAX_PATH];
			strcpy(pathCopy, path);
			strcat(pathCopy, screenshots);

			int wchars_num_copy = MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, NULL, 0);
			wchar_t* wstr_copy = new wchar_t[wchars_num_copy];
			MultiByteToWideChar(CP_UTF8, 0, pathCopy, -1, wstr_copy, wchars_num_copy);
			SHCreateDirectory(NULL, wstr_copy);
			delete[] wstr_copy;
		}
	}
	delete[] wstr;
}

std::replace(g_documentsPath.begin(), g_documentsPath.end(), '\\', '/');
	
#endif

#endif

	g_logger = new Logger();
	g_databaseManager = new DatabaseManager();
	g_databaseManager->init();
	g_resourceManager = new ResourceManager();
	g_resourceManager->init();
	g_textProvider = new TextProvider();
	g_textProvider->reload();
	g_inputController = new InputController();
	g_inputController->init();

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