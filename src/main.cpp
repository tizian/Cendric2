#include "global.h"
#include "Game.h"
#include "ResourceManager.h"
#include "InputController.h"

int main(int argc, char* argv[])
{
	g_resourceManager = new ResourceManager();
	g_resourceManager->init();
	g_inputController = new InputController();
	g_inputController->init();

	Game* game = new Game();
	game->run();
	delete game;

	delete g_resourceManager;
	delete g_inputController;

	return 0;
}