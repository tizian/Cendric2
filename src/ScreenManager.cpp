#include "stdafx.h"

ScreenManager::ScreenManager() {}

ScreenManager::ScreenManager(Screen *initialScreen)
{
	m_currentScreen = initialScreen;
}

void ScreenManager::update(float dt)
{
	//cout << "(ScreenManager::update): current screen is " << m_currentScreen << endl;
	m_currentScreen = m_currentScreen->update(dt);
}

void ScreenManager::render(sf::RenderTarget &renderTarget) const
{
	m_currentScreen->render(renderTarget);
}

Screen *ScreenManager::getCurrentScreen()
{
	return m_currentScreen;
}
