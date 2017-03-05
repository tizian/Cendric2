#include "Screens/LoadingScreen.h"
#include "Screens/ScreenManager.h"

using namespace std;

LoadingScreen::LoadingScreen(CharacterCore* core) : Screen(core) {
	if (core->getData().isInLevel) {
		m_levelToLoad = new LevelScreen(core->getData().currentLevel, getCharacterCore());
	}
	else {
		core->replaceForcedMap();
		m_mapToLoad = new MapScreen(core->getData().currentMap, getCharacterCore());
	}
}

void LoadingScreen::execUpdate(const sf::Time& frameTime) {
	// return once to render this screen.
	if (!m_isRendered) {
		m_isRendered = true;
		m_screenManager->clearBackupScreen();
		return;
	}

	if (m_levelToLoad != nullptr) {
		m_levelToLoad->load();
	}
	else if (m_mapToLoad != nullptr) {
		m_mapToLoad->load();
	}

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

void LoadingScreen::render(sf::RenderTarget& renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderTarget.draw(*m_title);
}

void LoadingScreen::execOnEnter(const Screen *previousScreen) {
	// background
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(GlobalResource::TEX_SCREEN_LOADING)));
	// title
	m_title = new BitmapText(g_textProvider->getText("Loading"));
	m_title->setTextStyle(TextStyle::Shadowed);
	m_title->setCharacterSize(GUIConstants::CHARACTER_SIZE_XXXL);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 50.f));
}

void LoadingScreen::execOnExit(const Screen *nextScreen) {
	delete m_title;
}