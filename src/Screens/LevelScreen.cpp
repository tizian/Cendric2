#include "Screens/LevelScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

LevelScreen::LevelScreen(LevelID levelID, CharacterCore* core) : Screen(core)
{
	m_levelID = levelID;
}

void LevelScreen::loadDynamicTiles()
{
	m_currentLevel.loadDynamicTiles(this);
}

void LevelScreen::load()
{
	delete m_characterCoreCopy;
	m_characterCoreCopy = new CharacterCore(*m_characterCore);

	if (!(m_currentLevel.load(m_levelID)))
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

	m_retryButton = new Button(sf::FloatRect(450, 410, 350, 50), ButtonOrnamentStyle::MEDIUM);
	m_retryButton->setText("BackToCheckpoint");
	m_retryButton->setVisible(false);
	addObject(m_retryButton);
	m_backToMenuButton = new Button(sf::FloatRect(450, 470, 350, 50), ButtonOrnamentStyle::MEDIUM);
	m_backToMenuButton->setText("BackToMenu");
	m_backToMenuButton->setVisible(false);
	addObject(m_backToMenuButton);

	m_overlaySprite = new sf::Sprite(*g_resourceManager->getTexture(ResourceID::Texture_screen_overlay));

	m_overlayText = new BitmapText(g_textProvider->getText("GamePaused"));
	m_overlayText->setCharacterSize(56);
	m_overlayText->setColor(sf::Color::Red);
	m_overlayText->setPosition(sf::Vector2f(std::max(0.f, (WINDOW_WIDTH - m_overlayText->getLocalBounds().width) / 2.f), 200.f));
}

void LevelScreen::cleanUp()
{
	m_currentLevel.dispose();
	delete m_characterCoreCopy;
	delete m_overlaySprite;
	delete m_overlayText;
	delete m_interface;
}

CharacterCore* LevelScreen::getCharacterCore() const
{
	return m_characterCoreCopy;
}

CharacterCore* LevelScreen::getOriginalCharacterCore() const
{
	return m_characterCore;
}

void LevelScreen::execOnEnter(const Screen *previousScreen)
{
	// nop
}

void LevelScreen::execOnExit(const Screen *nextScreen)
{
	cleanUp();
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
		m_overlaySprite->setTexture(*g_resourceManager->getTexture(ResourceID::Texture_screen_gameover));
		m_overlayText->setString(g_textProvider->getText("YouDied"));
		m_overlayText->setPosition(sf::Vector2f(std::max(0.f, (WINDOW_WIDTH - m_overlayText->getLocalBounds().width) / 2.f), 200.f));
		m_retryButton->setVisible(true);
		m_backToMenuButton->setVisible(true);
	}

	if (m_retryButton->isClicked())
	{
		return new LoadingScreen(m_characterCore);
	}

	if (m_backToMenuButton->isClicked())
	{
		return new MenuScreen(m_characterCore);
	}

	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);
	if (!m_retryButton->isVisible())
	{
		m_interface->update(frameTime);
	}

	if (!g_inputController->isActionLocked() && g_inputController->isKeyJustPressed(Key::Escape))
	{
		if (m_retryButton->isVisible())
		{
			m_retryButton->setVisible(false);
			m_backToMenuButton->setVisible(false);
		}
		else
		{
			m_retryButton->setVisible(true);
			m_backToMenuButton->setVisible(true);
		}
	}

	if (m_isGameOver || !m_retryButton->isVisible())
	{
		LevelExitBean* bean = m_currentLevel.checkLevelExit((*m_mainChar->getBoundingBox()));
		if (bean == nullptr)
		{
			updateObjects(GameObjectType::_Enemy, frameTime);
			if (!m_isGameOver) updateObjects(GameObjectType::_LevelItem, frameTime);
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
			return new LoadingScreen(m_characterCore);
		}
	}
	return this;
}

void LevelScreen::render(sf::RenderTarget &renderTarget)
{
	// don't render dynamic tiles here, they are rendered in the level.
	m_currentLevel.drawBackground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	// ASSURE that at this point, the view is the correct game view
	sf::View oldView = renderTarget.getView();
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
	m_interface->render(renderTarget); // this will set the view to the default view!

	if (m_retryButton->isVisible())
	{
		renderTarget.draw(*m_overlaySprite);
		renderTarget.draw(*m_overlayText);
	}
		
	renderObjects(GameObjectType::_Button, renderTarget);

	// reset the view for the updates
	renderTarget.setView(oldView);
}