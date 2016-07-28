#include "DialogueTool.h"
#include "MainWindow.h"

void DialogueTool::run() {

	// init window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Cendric Dialogue Tool");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	// init main controller
	MainWindow mainWindow;

	// run
	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(deltaClock.restart());

		mainWindow.update();
		window.clear();
		mainWindow.render();
		window.display();
	}

	ImGui::SFML::Shutdown();
}