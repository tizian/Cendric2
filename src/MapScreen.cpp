#include "MapScreen.h"

using namespace std;

MapScreen::MapScreen(ResourceID map)
{
	m_errorOccurred = !(m_currentMap.load(map));
	m_mainChar = new MapMainCharacter(&m_currentMap);
	addObject(m_mainChar);
}

Screen* MapScreen::update(sf::Time frameTime)
{
	if (m_errorOccurred)
	{
		return new ErrorScreen();
	}
	ResourceID id = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
	if (id == ResourceID::Void)
	{
		return Screen::update(frameTime);
	} 
	else
	{ 
		return new LoadingScreen(ScreenID::Screen_game, ResourceID::Level_testlevel);
	}
}

void MapScreen::onEnter(Screen *previousScreen)
{
}

void MapScreen::onExit(Screen *nextScreen)
{
	m_currentMap.dispose();
	delete m_mainChar;
}

void MapScreen::render(sf::RenderTarget &renderTarget)
{
	m_currentMap.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	Screen::render(renderTarget);
}