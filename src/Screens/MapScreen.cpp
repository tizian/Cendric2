#include "Screens/MapScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

const char* QUICKSAVE_LOCATION = "saves/quicksave.sav";

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
	if (g_inputController->isKeyJustPressed(Key::Escape))
	{
		// store pos & go back to menu screen
		m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
		return new MenuScreen(m_characterCore);
	}
	if (g_inputController->isKeyJustPressed(Key::Quickload))
	{
		// store pos & go back to menu screen
		CharacterCore* newCharacterCore = new CharacterCore();
		if (!newCharacterCore->load(QUICKSAVE_LOCATION))
		{
			// no quicksave exists
			setTooltipText(L"No quicksave exists.", sf::Vector2f(10.f, 10.f), sf::Color::Cyan, true);
			delete newCharacterCore;
		}
		else
		{
			delete m_characterCore;
			m_characterCore = newCharacterCore;
			return new LoadingScreen(m_characterCore->getData().currentMap, m_characterCore);
		}
	}
	if (g_inputController->isKeyJustPressed(Key::Quicksave))
	{
		m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
		m_characterCore->save(QUICKSAVE_LOCATION);
		setTooltipText(L"Saved file to: saves/quicksave.sav", sf::Vector2f(10.f, 10.f), sf::Color::Cyan, true);
	}
	LevelID id = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
	if (id == LevelID::Void || m_isOnLevelEntry)
	{
		m_isOnLevelEntry = (id != LevelID::Void);
		updateObjects(GameObjectType::_MainCharacter, frameTime);
		updateTooltipText(frameTime);
		return this;
	} 
	else
	{ 
		m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
		return new LoadingScreen(id, getCharacterCore());
	}
}

void MapScreen::execOnEnter(const Screen *previousScreen)
{
	m_mainChar = new MapMainCharacter(&m_currentMap);
	if (m_characterCore->getData().currentMap != m_currentMap.getID() || !m_characterCore->isLoaded())
	{
		m_characterCore->setMap(m_currentMap.getStartPos(), m_currentMap.getID());
	}
	m_mainChar->setCharacterCore(getCharacterCore());
	addObject(GameObjectType::_MainCharacter, m_mainChar);
}

void MapScreen::execOnExit(const Screen *nextScreen)
{
	m_currentMap.dispose();
}

void MapScreen::render(sf::RenderTarget &renderTarget)
{
	m_currentMap.drawBackground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	m_currentMap.drawForeground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderTooltipText(renderTarget);
}