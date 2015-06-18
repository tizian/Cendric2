#include "Screens/GameScreen.h"
#include "Screens/LoadingScreen.h"

using namespace std;

GameScreen::GameScreen(LevelID levelID, CharacterCore* core) : Screen(core), m_interface(core)
{
	m_levelID = levelID;
}

void GameScreen::execOnEnter(const Screen *previousScreen)
{
	if (!(m_currentLevel.load(m_levelID, this)))
	{
		string filename(g_resourceManager->getFilename(m_levelID));
		string errormsg = filename + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}
	LevelMainCharacterLoader loader;
	m_mainChar = loader.loadMainCharacter(this, &m_currentLevel);
	m_currentLevel.loadAfterMainChar(this);
	loader.loadEquipment(this);
	m_interface.setSpellManager(m_mainChar->getSpellManager());
}

void GameScreen::execOnExit(const Screen *nextScreen)
{
	m_currentLevel.dispose();
}

Screen* GameScreen::update(const sf::Time& frameTime)
{
	LevelExitBean* bean = m_currentLevel.checkLevelExit((*m_mainChar->getBoundingBox()));
	if (bean == nullptr || m_isOnLevelExit)
	{
		m_isOnLevelExit = (bean != nullptr);
		updateTooltipText(frameTime);
		updateObjects(GameObjectType::_MainCharacter, frameTime);
		updateObjects(GameObjectType::_LevelEquipment, frameTime);
		updateObjects(GameObjectType::_Spell, frameTime);
		updateObjects(GameObjectType::_DynamicTile, frameTime);
		updateObjects(GameObjectType::_LevelItem, frameTime);
		updateObjects(GameObjectType::_Enemy, frameTime);
		m_interface.update(frameTime);
		deleteDisposedObjects();
		return this;
	}
	else
	{
		m_characterCore->setMap(bean->mapSpawnPoint, bean->map);
		delete bean;
		return new LoadingScreen(m_characterCore->getData().currentMap, m_characterCore);
	}
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	renderTooltipText(renderTarget);
	// don't render dynamic tiles here, they are rendered in the level.
	m_currentLevel.drawBackground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	// ASSURE that at this point, the view is the correct game view
	renderObjects(GameObjectType::_LevelItem, renderTarget);
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_LevelEquipment, renderTarget);
	renderObjects(GameObjectType::_Enemy, renderTarget);
	renderObjects(GameObjectType::_Spell, renderTarget);
	m_currentLevel.drawForeground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	m_interface.render(renderTarget);
}