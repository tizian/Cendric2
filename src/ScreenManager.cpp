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

	if (m_nextScreen != nullptr)
	{
		cout << "(ScreenManager::update): Change Screen" << endl;
		//m_currentScreen->onExit(m_nextScreen);
		m_currentScreen = m_nextScreen;
		m_currentScreen->setScreenManager(this);
		m_nextScreen = nullptr;
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

void ScreenManager::changeScreenTo(Screen *nextScreen)
{
	cout << "(ScreenManager::changeScreenTo): Change Screen" << endl;
	m_nextScreen = nextScreen;
	cout << "(ScreenManager::changeScreenTo): Next Screen: " << m_nextScreen << endl;
	// m_currentScreen->onExit(m_nextScreen);


	//cout << "changing current screen (" << m_currentScreen << ") to " << nextScreen << endl;
	// TODO: why doesn't this work?!
	//Screen *previousScreen = m_currentScreen;
	//previousScreen->onExit(nextScreen);
	//m_currentScreen = nextScreen;
	//m_currentScreen->setScreenManager(this);
	//m_currentScreen->onEnter(previousScreen);

	// delete previousState;	// TODO(tizian): Memory leak?
}