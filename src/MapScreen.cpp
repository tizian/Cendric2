#include "MapScreen.h"

using namespace std;

MapScreen::MapScreen(ResourceID map)
{
	m_currentMap.load(map);
	m_mainChar = new MapMainCharacter(&m_currentMap);
	addObject(m_mainChar);
}

MapScreen::~MapScreen()
{
	m_currentMap.dispose();
	delete m_mainChar;
}

Screen* MapScreen::update(sf::Time frameTime)
{
	ResourceID id = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
	if (id == ResourceID::Void)
	{
		return Screen::update(frameTime);
	} 
	else
	{
		return new LoadingScreen(ScreenID::Screen_game, id);
	}
}

void MapScreen::render(sf::RenderTarget &renderTarget)
{
	m_currentMap.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	Screen::render(renderTarget);
}