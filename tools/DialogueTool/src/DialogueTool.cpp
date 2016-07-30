#include "DialogueTool.h"
#include "GUI/MainWindow.h"
#include "ApplicationState.h"

void DialogueTool::run() {

	// init window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Cendric Dialogue Tool");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	// init main controller
	MainWindow mainWindow;

	// run
	sf::Clock deltaClock;
	bool isRunning = true;
	while (isRunning) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				isRunning = false;
			}
		}

		ImGui::SFML::Update(deltaClock.restart());
		
		mainWindow.update();
		window.clear(sf::Color(50, 50, 60));
		mainWindow.render();
		window.display();

		isRunning = isRunning && !g_state->isQuitRequested();
	}

	window.close();
	ImGui::SFML::Shutdown();
}