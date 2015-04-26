#include "Screens\GameScreen.h"

using namespace std;

GameScreen::GameScreen(ResourceID levelID)
{
	m_levelID = levelID;
}

void GameScreen::execOnEnter(Screen *previousScreen)
{
	if (!(m_currentLevel.load(m_levelID, this)))
	{
		string filename(g_resourceManager->getFilename(m_levelID));
		string errormsg = filename + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
	}

	m_mainChar = new MainCharacter(&m_currentLevel);
	m_mainChar->setScreen(this);
	addObject(GameObjectType::_MainCharacter, m_mainChar);
	IceStaff* staff = new IceStaff();
	staff->loadWeapon(m_mainChar);
	addObject(GameObjectType::_Weapon, staff);
}

void GameScreen::execOnExit(Screen *nextScreen)
{
	m_currentLevel.dispose();
}

Screen* GameScreen::update(sf::Time frameTime)
{
	updateObjects(GameObjectType::_MainCharacter, frameTime);
	updateObjects(GameObjectType::_Weapon, frameTime);
	updateObjects(GameObjectType::_Spell, frameTime);
	updateObjects(GameObjectType::_DynamicTile, frameTime);
	deleteDisposedObjects();
	return this;
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	// parallax, maybe forground + background layers?
	// don't render dynamic tiles, they are rendered in the level.
	m_currentLevel.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_Weapon, renderTarget);
	renderObjects(GameObjectType::_Spell, renderTarget);
}