#include "Screens/GameScreen.h"

using namespace std;

GameScreen::GameScreen(LevelID levelID, CharacterCore* core) : Screen(core), m_interface(core)
{
	m_levelID = levelID;
}

void GameScreen::execOnEnter(const Screen *previousScreen)
{
	LevelMainCharacterLoader loader;
	m_mainChar = loader.loadMainCharacter(this, &m_currentLevel);
	loader.loadEquipment(this);
	if (!(m_currentLevel.load(m_levelID, this)))
	{
		string filename(g_resourceManager->getFilename(m_levelID));
		string errormsg = filename + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
	}
	m_interface.setSpellManager(m_mainChar->getSpellManager());
}

void GameScreen::execOnExit(const Screen *nextScreen)
{
	m_currentLevel.dispose();
}

Screen* GameScreen::update(const sf::Time& frameTime)
{
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

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	renderTooltipText(renderTarget);
	// parallax, maybe forground + background layers?
	// don't render dynamic tiles, they are rendered in the level.
	m_currentLevel.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	// ASSURE that at this point, the view is the correct game view
	renderObjects(GameObjectType::_LevelItem, renderTarget);
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_LevelEquipment, renderTarget);
	renderObjects(GameObjectType::_Enemy, renderTarget);
	renderObjects(GameObjectType::_Spell, renderTarget);
	m_interface.render(renderTarget);
}