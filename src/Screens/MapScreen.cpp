#include "Screens/MapScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

MapScreen::MapScreen(const std::string& mapID, CharacterCore* core) : Screen(core)
{
	m_mapID = mapID;
}

Screen* MapScreen::update(const sf::Time& frameTime)
{
	// handle case where a dialogue is open
	if (m_dialogueWindow != nullptr)
	{
		if (!m_dialogueWindow->updateDialogue())
		{
			delete m_dialogueWindow;
			m_dialogueWindow = nullptr;
		}
		return this;
	}
	else
	{
		m_interface->update(frameTime);
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
			if (!newCharacterCore->quickload())
			{
				// no quicksave exists
				setTooltipText(g_textProvider->getText("NoQuicksaveExists"), sf::Color::Red, true);
				delete newCharacterCore;
			}
			else
			{
				delete m_characterCore;
				m_characterCore = newCharacterCore;
				return new LoadingScreen(m_characterCore);
			}
		}
		if (g_inputController->isKeyJustPressed(Key::Quicksave))
		{
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			m_characterCore->quicksave();
			setTooltipText(g_textProvider->getText("GameSaved"), sf::Color::Green, true);
		}
		MapExitBean* bean = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
		if (bean == nullptr || m_isOnLevelEntry)
		{
			m_isOnLevelEntry = (bean != nullptr);
			updateObjects(GameObjectType::_MainCharacter, frameTime);
			updateObjects(GameObjectType::_NPC, frameTime);
			updateTooltipText(frameTime);
			deleteDisposedObjects();
			return this;
		}
		else
		{
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			m_characterCore->setLevel(bean->levelSpawnPoint, bean->levelID);
			delete bean;
			return new LoadingScreen(getCharacterCore());
		}
	}
}

void MapScreen::load()
{
	if (!(m_currentMap.load(m_mapID)))
	{
		string errormsg = m_mapID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}
	m_mainChar = new MapMainCharacter(&m_currentMap);
	m_mainChar->setCharacterCore(getCharacterCore());
	addObject(m_mainChar);
	m_currentMap.loadAfterMainChar(this);

	m_interface = new MapInterface(m_characterCore);
}

void MapScreen::execOnEnter(const Screen *previousScreen)
{
	// nop
}

void MapScreen::execOnExit(const Screen *nextScreen)
{
	m_currentMap.dispose();
	delete m_interface;
}

void MapScreen::setDialogue(const NPCBean& bean)
{
	if (m_dialogueWindow != nullptr)
	{
		delete m_dialogueWindow;
	}

	m_dialogueWindow = new DialogueWindow();
	m_dialogueWindow->load(bean, m_characterCore);
}

void MapScreen::render(sf::RenderTarget &renderTarget)
{
	// game view
	m_currentMap.drawBackground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_NPC, renderTarget);
	m_currentMap.drawForeground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderObjectsAfterForeground(GameObjectType::_MainCharacter, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_NPC, renderTarget);
	
	// default view
	sf::View oldView = renderTarget.getView();
	renderTooltipText(renderTarget);
	m_interface->render(renderTarget); // this will set the view to the default view!
	// render the dialogue window. 
	if (m_dialogueWindow != nullptr)
	{
		m_dialogueWindow->render(renderTarget);
	}
	renderTarget.setView(oldView);
}