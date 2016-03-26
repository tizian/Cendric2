#include "ScreenManager.h"
#include "Screens/WorldScreen.h"

ScreenManager::ScreenManager(Screen* initialScreen) : m_isErrorScreen(false) {
	m_currentScreen = initialScreen;
	m_currentScreen->setScreenManager(this);
	m_currentScreen->onEnter(nullptr);
}

ScreenManager::~ScreenManager() {
	if (m_currentScreen != nullptr) {
		m_currentScreen->onExit(nullptr);
		delete m_currentScreen->getCharacterCore();
		delete m_currentScreen;
	}
	if (m_backUpScreen != nullptr && m_backUpScreen != m_currentScreen) {
		m_backUpScreen->onExit(nullptr);
		delete m_backUpScreen->getCharacterCore();
		delete m_backUpScreen;
	}
	if (m_nextScreen != nullptr && m_nextScreen != m_currentScreen && m_nextScreen != m_backUpScreen) {
		m_nextScreen->onExit(nullptr);
		delete m_nextScreen->getCharacterCore();
		delete m_nextScreen;
	}
}

void ScreenManager::update(const sf::Time& frameTime) {
	m_currentScreen->update(frameTime);

	if (m_nextScreen != nullptr) {
		if (!m_isBackupRequested) {
			m_currentScreen->onExit(m_nextScreen);
			if (m_isResumeBackupScreen) {
				m_isResumeBackupScreen = false;
			}
			else {
				m_nextScreen->onEnter(m_currentScreen);
			}
			delete m_currentScreen;
		}
		else {
			delete m_backUpScreen;
			m_backUpScreen = m_currentScreen;
			m_nextScreen->onEnter(m_currentScreen);
		}

		m_currentScreen = m_nextScreen;
		m_nextScreen = nullptr;
	}
}

void ScreenManager::setNextScreen(Screen* nextScreen, bool backupCurrentScreen) {
	delete m_nextScreen;
	m_isBackupRequested = backupCurrentScreen;
	m_nextScreen = nextScreen;
	m_nextScreen->setScreenManager(this);
}

void ScreenManager::resumeBackupScreen() {
	if (m_backUpScreen == nullptr) return;

	setNextScreen(m_backUpScreen, false);
	m_backUpScreen = nullptr;
	m_isResumeBackupScreen = true;
}

void ScreenManager::clearBackupScreen() {
	if (m_backUpScreen == nullptr) return;
	m_backUpScreen->execOnExit(nullptr);
	delete m_backUpScreen;
	m_backUpScreen = nullptr;
}

void ScreenManager::setErrorScreen() {
	if (m_isErrorScreen) return;
	m_currentScreen->onExit(nullptr);
	Screen* nextScreen = new ErrorScreen(m_currentScreen->getCharacterCore());
	nextScreen->onEnter(m_currentScreen);
	delete m_currentScreen;
	m_currentScreen = nextScreen;
	m_isErrorScreen = true;
}

void ScreenManager::requestQuit() {
	m_isQuitRequested = true;
}

void ScreenManager::render(sf::RenderTarget &renderTarget) {
	m_currentScreen->render(renderTarget);
}

