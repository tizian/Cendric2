#include "stdafx.h"

ScreenManager::ScreenManager() {}

ScreenManager::ScreenManager(Screen *initialScreen)
{
	m_currentScreen = initialScreen;
	m_currentScreen->setScreenManager(this);
}

void ScreenManager::update(float dt)
{
	//cout << "(ScreenManager::update): current screen is " << m_currentScreen << endl;
	m_currentScreen->update(dt);
}

void ScreenManager::render(sf::RenderTarget &renderTarget) const
{
	m_currentScreen->render(renderTarget);
}

Screen *ScreenManager::getCurrentScreen()
{
	return m_currentScreen;
}

void ScreenManager::changeScreenTo(Screen *nextScreen)
{
	cout << "changing current screen (" << m_currentScreen << ") to " << nextScreen << endl;
	// TODO: why doesn't this work?!
	Screen *previousScreen = m_currentScreen;
	//previousScreen->onExit(nextScreen);
	m_currentScreen = nextScreen;
	m_currentScreen->setScreenManager(this);
	//m_currentScreen->onEnter(previousScreen);

	// delete previousState;	// TODO(tizian): Memory leak?
}