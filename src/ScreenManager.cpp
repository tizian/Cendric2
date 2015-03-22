#include "stdafx.h"

ScreenManager::ScreenManager() {}

ScreenManager::ScreenManager(Screen *initialScreen)
{
	m_currentScreen = initialScreen;
}

void ScreenManager::update(float dt)
{
	Screen* nextScreen = m_currentScreen->update(dt);
	if (m_currentScreen != nextScreen) {
		m_currentScreen->onExit(nextScreen);
		nextScreen->onEnter(m_currentScreen);
		delete m_currentScreen;
		m_currentScreen = nextScreen;
	}
}

void ScreenManager::render(sf::RenderTarget &renderTarget) const
{
	m_currentScreen->render(renderTarget);
}

Screen *ScreenManager::getCurrentScreen()
{
	return m_currentScreen;
}
