#include "Game.h"

Game::Game() 
{
	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cendric");
	//m_mainWindow.setFramerateLimit(g_resourceManager->getMaxFPS());
	//m_mainWindow.setVerticalSyncEnabled(true);
	g_inputController->setWindow(&m_mainWindow);
	m_running = true;

	if (g_resourceManager->getConfiguration().isDebugMode)
	{
		CharacterCore* core = new CharacterCore();
		if (!(core->load(CharacterCore::DEBUGSAVE_LOCATION)))
		{
			std::string errormsg = std::string(CharacterCore::DEBUGSAVE_LOCATION) + ": save file corrupted!";
			g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
			m_screenManager = new ScreenManager(new SplashScreen());
			delete core;
		}
		else
		{
			m_screenManager = new ScreenManager(new LoadingScreen(core));
		}
	}
	else
	{
		m_screenManager = new ScreenManager(new SplashScreen());
	}
}

Game::~Game()
{
	delete m_screenManager;
}

void Game::run()
{
	sf::Clock frameClock;
	sf::Time frameTime;
	frameTime = frameClock.restart();
	while (m_running)
	{
		sf::Event e;
		sf::Time deltaTime;
		
		// input
		g_inputController->update();

		// don't count this loop into the frametime!
		deltaTime = frameClock.restart();
		while (m_mainWindow.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				m_running = false;
			}
			else if (e.type == sf::Event::Resized)
			{
				g_inputController->setCurrentWindowSize(e.size.width, e.size.height);
			}
			else if (e.type == sf::Event::TextEntered)
			{
				g_inputController->readUnicode(e.text.unicode);
			}
			else if (e.type == sf::Event::KeyPressed)
			{
				g_inputController->setLastPressedKey(e.key.code);
			}
		}
		
		frameClock.restart();
		// hard bound: dt should not exeed 50ms (20fps)
		if (deltaTime.asMilliseconds() > 50)
		{
			frameTime = sf::milliseconds(50);
			g_logger->logInfo("Game Loop", "Frame time just exceeded 50ms and is set down to 50ms. Its time was (ms): " + std::to_string(deltaTime.asMilliseconds()));
		}
		else
		{
			frameTime = deltaTime;
		}

		// game updates
		m_screenManager->update(frameTime);
		if (m_screenManager->getCurrentScreen()->isQuitRequested())
		{
			m_running = false;
		}
		if (g_resourceManager->pollError()->first != ErrorID::Void)
		{
			m_screenManager->setErrorScreen();
		}

		// render
		m_mainWindow.clear();
		m_screenManager->render(m_mainWindow);
		if (g_resourceManager->getConfiguration().isDebugRendering)
		{
			showFPSText(m_mainWindow, frameTime.asSeconds());
		}
		
		m_mainWindow.display();
	}

	m_mainWindow.close();
}

void Game::showFPSText(sf::RenderTarget& target, float frameTimeSeconds)
{
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());
	m_fpsList.push_back(frameTimeSeconds);
	if (m_fpsList.size() > FPS_AVERAGE_NR)
	{
		m_fpsList.pop_front();
	}
	// calc average
	float sum = 0.f;
	for (float f : m_fpsList)
	{
		sum += f;
	}
	int fps = static_cast<int>(1.f / (sum / FPS_AVERAGE_NR));

	BitmapText fpsText = BitmapText(
		"FPS: " + std::to_string(fps),
		(*g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default)));
	fpsText.setColor(sf::Color::Red);
	fpsText.setPosition(sf::Vector2f(1050.f, 10.f));
	fpsText.setCharacterSize(20);

	target.draw(fpsText);
	target.setView(oldView);
}