#include "DialogueTool.h"
#include "GUI/MainWindow.h"
#include "ApplicationState.h"

void DialogueTool::run() {

	// init window
	sf::RenderWindow window(sf::VideoMode({ 1200, 720 }), "Cendric Dialogue Tool");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	// init main controller
	MainWindow mainWindow;

	// run
	sf::Clock deltaClock;
	bool isRunning = true;
	while (isRunning) {
		while (const std::optional event  = window.pollEvent()) {
			if (event) {
				ImGui::SFML::ProcessEvent(window, *event);

				if (event->is<sf::Event::Closed>()) {
					isRunning = false;
				}
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		
		mainWindow.update();
		window.clear(sf::Color(50, 50, 60));
		mainWindow.render();
		window.display();

		isRunning = isRunning && !g_state->isQuitRequested();
	}

	window.close();
	ImGui::SFML::Shutdown();
}
