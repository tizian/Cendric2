#include "Screens\MapScreen.h"

using namespace std;

MapScreen::MapScreen(ResourceID map)
{
	if (!(m_currentMap.load(map)))
	{
		string filename(g_resourceManager->getFilename(map));
		string errormsg = filename + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
	}
}

Screen* MapScreen::update(const sf::Time& frameTime)
{
	ResourceID id = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
	if (id == ResourceID::Void)
	{
		updateObjects(GameObjectType::_MainCharacter, frameTime);
		return this;
	} 
	else
	{ 
		return new LoadingScreen(ScreenID::Screen_game, ResourceID::Level_testlevel);
	}
}

void MapScreen::execOnEnter(const Screen *previousScreen)
{
	m_mainChar = new MapMainCharacter(&m_currentMap);
	addObject(GameObjectType::_MainCharacter, m_mainChar);
}

void MapScreen::execOnExit(const Screen *nextScreen)
{
	m_currentMap.dispose();
}

void MapScreen::render(sf::RenderTarget &renderTarget)
{
	m_currentMap.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
}