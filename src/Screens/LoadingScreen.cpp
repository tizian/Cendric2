#include "Screens/LoadingScreen.h"
#include "Screens/ScreenManager.h"

LoadingScreen::LoadingScreen(CharacterCore* core) : Screen(core) {
	if (core->getData().isInLevel) {
		m_worldToLoad = new LevelScreen(core->getData().currentLevel, getCharacterCore());
	}
	else {
		core->replaceForcedMap();
		m_worldToLoad = new MapScreen(core->getData().currentMap, getCharacterCore());
	}
	m_blackRect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	m_timeToBlack = sf::seconds(0.5f);

	m_blackRect.setFillColor(g_resourceManager->getConfiguration().isMultithreading ?
		COLOR_TRANSPARENT :
		COLOR_TRANS_BLACK
	);
}

void LoadingScreen::execUpdate(const sf::Time& frameTime) {
	// return once to render this screen.
	if (!m_isRendered) {
		m_isRendered = true;
		m_screenManager->clearBackupScreen();

		if (g_resourceManager->getConfiguration().isMultithreading) {
			// start async thread
			m_threadDone = false;
			m_thread = new std::thread([this] {
				this->loadAsync();
				this->m_threadDone = true;
			});
		} 
		else {
			loadAsync();
			m_threadDone = true;
		}
	}

	if (!m_threadDone) {
		updateTime(m_timeToBlack, frameTime);
		m_blackRect.setFillColor(sf::Color(0, 0, 0, 255 - static_cast<sf::Uint8>(std::floor(m_timeToBlack.asMilliseconds() / 500.f * 255.f))));
		return;
	}

	if (m_thread) {
		m_thread->join();
		delete m_thread;
	}

	if (g_resourceManager->pollError()->first == ErrorID::VOID) m_worldToLoad->loadSync();
	setNextScreen(m_worldToLoad);
	m_characterCore->autosave();
}

void LoadingScreen::loadAsync() const {
	m_worldToLoad->loadAsync();
}

void LoadingScreen::render(sf::RenderTarget& renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderTarget.draw(m_blackRect);
}

void LoadingScreen::execOnEnter() {
	g_renderTexture->setActive(true);
	m_texture = new sf::Texture(g_renderTexture->getTexture());
	m_screenSprite.setTexture(*m_texture);
}

void LoadingScreen::execOnExit() {
	delete m_texture;
}
