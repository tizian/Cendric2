#include "Screens/LoadingScreen.h"
#include "Screens/ScreenManager.h"

LoadingScreen::LoadingScreen(CharacterCore* core) : Screen(core) {
	if (core->getData().isInLevel) {
		m_levelToLoad = new LevelScreen(core->getData().currentLevel, getCharacterCore());
	}
	else {
		core->replaceForcedMap();
		m_mapToLoad = new MapScreen(core->getData().currentMap, getCharacterCore());
	}
	m_blackRect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	m_blackRect.setFillColor(COLOR_TRANSPARENT);
	m_timeToBlack = sf::seconds(0.5f);
}

void LoadingScreen::execUpdate(const sf::Time& frameTime) {
	// return once to render this screen.
	if (!m_isRendered) {
		m_isRendered = true;
		m_screenManager->clearBackupScreen();
		
		// start async thread
		m_threadDone = false;
		m_thread = new std::thread([this] {
			this->loadAsync();
			this->m_threadDone = true;
		});
	}

	if (!m_threadDone) {
		updateTime(m_timeToBlack, frameTime);
		m_blackRect.setFillColor(sf::Color(0, 0, 0, 255 - static_cast<sf::Uint8>(std::floor(m_timeToBlack.asMilliseconds() / 500.f * 255.f))));
		return;
	}

	m_thread->join();
	delete m_thread;

	if (m_mapToLoad != nullptr) {
		if (g_resourceManager->pollError()->first == ErrorID::VOID) m_mapToLoad->loadForRenderTexture();
		setNextScreen(m_mapToLoad);
		if (m_mapToLoad->getCharacterCore()->isNewGame()) {
			m_mapToLoad->addScreenOverlay(ScreenOverlay::createQuestScreenOverlay("who_am_i", QuestState::Started));
		}
		m_characterCore->autosave();
		return;
	}
	if (m_levelToLoad != nullptr) {
		if (g_resourceManager->pollError()->first == ErrorID::VOID) m_levelToLoad->loadForRenderTexture();
		setNextScreen(m_levelToLoad);
		m_characterCore->autosave();
		return;
	}

	g_resourceManager->setError(ErrorID::Error_dataCorrupted, "No level or map to load. Aborting.");
}

void LoadingScreen::loadAsync() const {
	if (m_levelToLoad != nullptr) {
		m_levelToLoad->load();
	}
	else if (m_mapToLoad != nullptr) {
		m_mapToLoad->load();
	}
}

void LoadingScreen::render(sf::RenderTarget& renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	//renderTarget.draw(m_screenSprite);
	//renderTarget.draw(m_blackRect);
}

void LoadingScreen::execOnEnter(Screen* previousScreen) {
	auto texture = new sf::Texture();
	texture->create(WINDOW_WIDTH, WINDOW_HEIGHT);
	//texture->update(*g_renderWindow);
	auto image = texture->copyToImage();
	delete texture;
	m_texture.loadFromImage(image);
	m_screenSprite.setTexture(m_texture);
}

void LoadingScreen::execOnExit(Screen*)
{
}
