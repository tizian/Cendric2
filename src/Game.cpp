#include "Game.h"
#include "Test/CendricTests.h"
#include "Misc/icon.h"
#ifdef STEAM
#include "steam-sdk/public/steam/steam_api.h"
#endif

#define CAPTION "Cendric"

sf::RenderTexture* g_renderTexture;

Game::Game() {
	m_renderTexture.resize({WINDOW_WIDTH, WINDOW_HEIGHT});
	reloadWindow();

	m_mainSprite->setTexture(m_renderTexture.getTexture());
	
	m_running = true;
	m_screenManager = new ScreenManager(new SplashScreen());
}

Game::~Game() {
	delete m_screenManager;
}

void Game::reloadWindow() {
	auto const displayMode = g_resourceManager->getConfiguration().displayMode;
	auto const windowMode = sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT});
	auto const videoMode = displayMode == DisplayMode::Window ? windowMode : sf::VideoMode::getDesktopMode();

	auto const scale = sf::Vector2f(videoMode.size.x / static_cast<float>(WINDOW_WIDTH), videoMode.size.y / static_cast<float>(WINDOW_HEIGHT));

	m_mainSprite->setScale(scale);

	switch (displayMode) {
	case DisplayMode::Fullscreen:
		m_mainWindow.create(videoMode, CAPTION, sf::State::Fullscreen);
		break;
	case DisplayMode::WindowedFullscreen:
	case DisplayMode::Window:
	default:
		m_mainWindow.create(videoMode, CAPTION);
	}

	auto& conf = g_resourceManager->getConfiguration();

	m_mainWindow.setMouseCursorVisible(false); // Hide cursor
	m_mainWindow.setVerticalSyncEnabled(conf.isVSyncEnabled);
	m_mainWindow.setFramerateLimit(conf.isFPSLimited ? conf.maxFPS : 0);

	m_renderTexture.setSmooth(conf.isSmoothing);

	conf.isWindowReload = false;

	g_inputController->setWindow(&m_mainWindow, &m_renderTexture, scale);
	g_renderTexture = &m_renderTexture;
}

void Game::run() {
	sf::Clock frameClock;
	sf::Time frameTime = frameClock.restart();
	g_inputController->notifyGamepadConnected();

	while (m_running) {
		// input
		g_inputController->update(frameTime);
		
		// don't count this loop into the frametime!
		sf::Time deltaTime = frameClock.restart();
		pollEvents();

		frameClock.restart();
		if (deltaTime.asSeconds() > MAX_FRAME_TIME) {
			frameTime = sf::seconds(MAX_FRAME_TIME);
			g_logger->logInfo("Game Loop", "Frame time just exceeded max frame time (" + std::to_string(MAX_FRAME_TIME) + "s). Its time was (ms): " + std::to_string(deltaTime.asMilliseconds()));
		}
		else {
			frameTime = deltaTime;
		}

		// game updates
#ifdef STEAM
		SteamAPI_RunCallbacks();
#endif // STEAM

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
		if (g_resourceManager->getConfiguration().isDebugRenderingOn) {
			showFPSText(m_renderTexture, frameTime.asSeconds());
		}

		m_renderTexture.display();
		m_mainWindow.draw(*m_mainSprite);
		g_inputController->getCursor().render(m_mainWindow);
		m_mainWindow.display();
	}

	m_mainWindow.close();
}

void Game::pollEvents() {
	while (const std::optional event = m_mainWindow.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			m_screenManager->requestQuit();
		}
		else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
			g_inputController->setCurrentWindowSize(resized->size.x, resized->size.y);
		}
		else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
			g_inputController->readUnicode(textEntered->unicode);
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			g_inputController->setLastPressedKey(keyPressed->code);
		}
		else if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>()) {
			g_inputController->setLastPressedGamepadAxis(*joystickMoved);
		}
		else if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>()) {
			g_inputController->setLastPressedGamepadButton(*joystickButtonPressed);
		}
		else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
			g_inputController->setMouseWheelScrollTicks(mouseWheelScrolled->delta);
		}
		else if (event->is<sf::Event::JoystickConnected>() || event->is<sf::Event::JoystickDisconnected>()) {
			g_inputController->notifyGamepadConnected();
		}
	}
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

void Game::runTests() {
	CendricTests::runTests();
}
