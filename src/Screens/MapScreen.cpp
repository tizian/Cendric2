#include "Screens/MapScreen.h"

using namespace std;

MapScreen::MapScreen(MapID mapID, CharacterCore* core) : Screen(core)
{
	if (!(m_currentMap.load(mapID)))
	{
		string filename(g_resourceManager->getFilename(mapID));
		string errormsg = filename + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
	}
}

Screen* MapScreen::update(const sf::Time& frameTime)
{
	LevelID id = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
	if (id == LevelID::Void)
	{
		updateObjects(GameObjectType::_MainCharacter, frameTime);
		return this;
	} 
	else
	{ 
		return new LoadingScreen(id, getCharacterCore());
	}
}

void MapScreen::execOnEnter(const Screen *previousScreen)
{
	m_mainChar = new MapMainCharacter(&m_currentMap);
	m_mainChar->setCharacterCore(getCharacterCore());
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