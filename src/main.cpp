#include "global.h"
#include "Game.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Logger.h"
#include "TextProvider.h"

int main(int argc, char* argv[])
{
	g_logger = new Logger();
	g_textProvider = new TextProvider();
	g_resourceManager = new ResourceManager();
	g_resourceManager->init();
	g_inputController = new InputController();
	g_inputController->init();

	Game* game = new Game();
	game->run();
	delete game;

	delete g_resourceManager;
	delete g_inputController;
	delete g_textProvider;
	delete g_logger;

	return 0;
}