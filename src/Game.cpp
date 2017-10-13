#include "Game.h"
#include "Misc/icon.h"

sf::RenderWindow* g_renderWindow;

Game::Game() {
	m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	reloadWindow();

	m_mainSprite.setTexture(m_renderTexture.getTexture());
	
	m_running = true;
	m_screenManager = new ScreenManager(new SplashScreen());
}

Game::~Game() {
	delete m_screenManager;
}

void Game::reloadWindow() {
	switch (g_resourceManager->getConfiguration().displayMode) {
	case DisplayMode::Fullscreen:
		m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cendric", sf::Style::Fullscreen);
		break;
	case DisplayMode::WindowedFullscreen:
		m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cendric", sf::Style::None);
		m_mainWindow.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
		m_mainWindow.setPosition(sf::Vector2i(0, 0));
		break;
	case DisplayMode::Window:
	default:
		m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cendric", sf::Style::Default);
	}

	auto& conf = g_resourceManager->getConfiguration();

	m_mainWindow.setMouseCursorVisible(false); // Hide cursor
	m_mainWindow.setVerticalSyncEnabled(conf.isVSyncEnabled);
	m_mainWindow.setIcon(cendric_icon.width, cendric_icon.height, cendric_icon.pixel_data);
	m_mainWindow.setFramerateLimit(conf.isFPSLimited ? conf.maxFPS : 0);

	m_renderTexture.setSmooth(conf.isSmoothing);

	conf.isWindowReload = false;

	g_inputController->setWindow(&m_mainWindow, &m_renderTexture);
	g_renderWindow = &m_mainWindow;
}

void Game::run() {
	sf::Clock frameClock;
	sf::Time frameTime = frameClock.restart();

	while (m_running) {
		sf::Event e;

		// input
		g_inputController->update(frameTime);
		
		// don't count this loop into the frametime!
		sf::Time deltaTime = frameClock.restart();
		while (m_mainWindow.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				m_screenManager->requestQuit();
			}
			else if (e.type == sf::Event::Resized) {
				g_inputController->setCurrentWindowSize(e.size.width, e.size.height);
			}
			else if (e.type == sf::Event::TextEntered) {
				g_inputController->readUnicode(e.text.unicode);
			}
			else if (e.type == sf::Event::KeyPressed) {
				g_inputController->setLastPressedKey(e.key.code);
			}
			else if (e.type == sf::Event::MouseWheelScrolled) {
				g_inputController->setMouseWheelScrollTicks(e.mouseWheelScroll.delta);
			}
		}

		frameClock.restart();
		if (deltaTime.asSeconds() > MAX_FRAME_TIME) {
			frameTime = sf::seconds(MAX_FRAME_TIME);
			g_logger->logInfo("Game Loop", "Frame time just exceeded max frame time (" + std::to_string(MAX_FRAME_TIME) + "s). Its time was (ms): " + std::to_string(deltaTime.asMilliseconds()));
		}
		else {
			frameTime = deltaTime;
		}

		// game updates
		m_screenManager->update(frameTime);
		g_resourceManager->updateMusic(frameTime);
		if (m_screenManager->isQuitRequested()) {
			m_running = false;
		}
		if (g_resourceManager->pollError()->first != ErrorID::VOID) {
			m_screenManager->setErrorScreen();
		}
		if (g_resourceManager->getConfiguration().isWindowReload) {
			reloadWindow();
		}

		// render
		m_mainWindow.clear();
		m_renderTexture.clear();
		m_screenManager->render(m_renderTexture);
		if (g_resourceManager->getConfiguration().isDebugRendering) {
			showFPSText(m_renderTexture, frameTime.asSeconds());
		}

		m_renderTexture.display();
		m_mainWindow.draw(m_mainSprite);
		g_inputController->getCursor().render(m_mainWindow);
		m_mainWindow.display();
	}

	m_mainWindow.close();
}

void Game::showFPSText(sf::RenderTarget& target, float frameTimeSeconds) {
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());
	m_fpsList.push_back(frameTimeSeconds);
	if (static_cast<int>(m_fpsList.size()) > FPS_AVERAGE_NR) {
		m_fpsList.pop_front();
	}
	// calc average
	float sum = 0.f;
	for (float f : m_fpsList) {
		sum += f;
	}
	auto const fps = static_cast<int>(1.f / (sum / FPS_AVERAGE_NR));

	BitmapText fpsText = BitmapText(
		"FPS: " + std::to_string(fps));
	fpsText.setColor(COLOR_BAD);
	fpsText.setPosition(sf::Vector2f(1050.f, 10.f));
	fpsText.setCharacterSize(16);

	target.draw(fpsText);
	target.setView(oldView);
}