#include "stdafx.h"

int main(int argc, char* argv[])
{
	g_resourceManager = new ResourceManager();
	g_resourceManager->init();

	Game game;
	game.run();

	delete g_resourceManager;

	return 0;
}