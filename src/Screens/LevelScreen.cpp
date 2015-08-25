#include "Screens/LevelScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

LevelScreen::LevelScreen(LevelID levelID, CharacterCore* core) : Screen(core)
{
	m_levelID = levelID;
}

void LevelScreen::execOnEnter(const Screen *previousScreen)
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
	m_interface = new LevelInterface(m_characterCore, m_mainChar);
	m_interface->setSpellManager(m_mainChar->getSpellManager());
}

void LevelScreen::execOnExit(const Screen *nextScreen)
{
	m_currentLevel.dispose();
	delete m_gameOverSprite;
	delete m_youDied;
	delete m_interface;
}

void LevelScreen::addBuffToInterface(BuffType type, const sf::IntRect& textureLocation, const sf::Time& duration) const
{
	m_interface->addBuff(type, textureLocation, duration);
}

void LevelScreen::reloadInventory()
{
	m_interface->reloadInventory();
}

Screen* LevelScreen::update(const sf::Time& frameTime)
{
	// handle game over
	if (!m_isGameOver && m_mainChar->isDead())
	{
		m_isGameOver = true;
		m_gameOverSprite = new sf::Sprite(*g_resourceManager->getTexture(ResourceID::Texture_screen_gameover));
		m_youDied = new BitmapText(g_textProvider->getText("YouDied"));
		m_youDied->setCharacterSize(56);
		m_youDied->setColor(sf::Color::Red);
		m_youDied->setPosition(sf::Vector2f(std::max(0.f, (WINDOW_WIDTH - m_youDied->getLocalBounds().width) / 2.f), 200.f));
		m_retryButton = new Button(sf::FloatRect(475, 410, 300, 50), ButtonOrnamentStyle::MEDIUM);
		m_retryButton->setText("RetryLevel");
		addObject(m_retryButton);
		m_backToMenuButton = new Button(sf::FloatRect(475, 470, 300, 50), ButtonOrnamentStyle::MEDIUM);
		m_backToMenuButton->setText("BackToMenu");
		addObject(m_backToMenuButton);
	}

	if (m_isGameOver && m_retryButton->isClicked())
	{
		return new LoadingScreen(m_characterCore->getData().currentLevel, m_characterCore);
	} 

	if (m_isGameOver && m_backToMenuButton->isClicked())
	{
		return new MenuScreen(m_characterCore);
	}

	LevelExitBean* bean = m_currentLevel.checkLevelExit((*m_mainChar->getBoundingBox()));
	if (bean == nullptr || m_isGameOver || !g_inputController->isKeyActive(Key::Up))
	{
		updateTooltipText(frameTime);
		m_interface->update(frameTime);
		updateObjects(GameObjectType::_Button, frameTime);
		updateObjects(GameObjectType::_Enemy, frameTime);
		updateObjects(GameObjectType::_LevelItem, frameTime);
		updateObjects(GameObjectType::_MainCharacter, frameTime);
		updateObjects(GameObjectType::_LevelEquipment, frameTime);
		updateObjects(GameObjectType::_Spell, frameTime);
		updateObjects(GameObjectType::_DynamicTile, frameTime);
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

void LevelScreen::render(sf::RenderTarget &renderTarget)
{
	// don't render dynamic tiles here, they are rendered in the level.
	m_currentLevel.drawBackground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	// ASSURE that at this point, the view is the correct game view
	renderObjects(GameObjectType::_LevelItem, renderTarget);
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_LevelEquipment, renderTarget);
	renderObjects(GameObjectType::_Enemy, renderTarget);
	renderObjects(GameObjectType::_Spell, renderTarget);

	m_currentLevel.drawForeground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());

	renderObjectsAfterForeground(GameObjectType::_LevelItem, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_MainCharacter, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_LevelEquipment, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Enemy, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_Spell, renderTarget);

	renderTooltipText(renderTarget);
	m_interface->render(renderTarget);

	if (m_isGameOver)
	{
		renderTarget.setView(renderTarget.getDefaultView());
		renderTarget.draw(*m_gameOverSprite);
		renderTarget.draw(*m_youDied);
		renderObjects(GameObjectType::_Button, renderTarget);
	}
}