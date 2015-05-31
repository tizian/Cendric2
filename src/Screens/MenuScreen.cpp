#include "Screens/MenuScreen.h"

using namespace std;

MenuScreen::MenuScreen(CharacterCore* core) : Screen(core)
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_menu)));
}

Screen* MenuScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || m_exitButton->isClicked())
	{
		// end the game
		m_requestQuit = true;
	}
	else if (m_newGameButton->isClicked())
	{
		// be lenient 
		delete m_characterCore;
		// we start a new game with an empty character core
		m_characterCore = new CharacterCore();
		m_characterCore->loadNew();
		m_startGameButton->setEnabled(true);
		setTooltipText("Loaded new game", sf::Vector2f(10.f, 10.f), sf::Color::Cyan, true);
	}
	else if (m_loadGameButton->isClicked())
	{
		// TODO the .sav files should be loaded in another screen.
		char* saveFilename = "saves/testsave.sav";
		
		delete m_characterCore;
		m_characterCore = new CharacterCore();
		if (!(m_characterCore->load(saveFilename)))
		{
			string errormsg = string(saveFilename) + ": save file corrupted!";
			g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		}
		m_startGameButton->setEnabled(true);
		setTooltipText("Loaded .sav file: " + string(saveFilename), sf::Vector2f(10.f, 10.f), sf::Color::Cyan, true);
	} 
	else if (m_startGameButton->isClicked() && m_characterCore != nullptr)
	{
		return new LoadingScreen(m_characterCore->getData().currentMap, m_characterCore);
	}
	updateTooltipText(frameTime);
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Undefined, frameTime);
	return this;
}

void MenuScreen::render(sf::RenderTarget &renderTarget) 
{
	renderTooltipText(renderTarget);
	setViewToStandardView(renderTarget);
	renderTarget.draw(m_screenSprite);
	renderObjects(GameObjectType::_Undefined, renderTarget);
	renderObjects(GameObjectType::_Button, renderTarget);

	sf::View oldView = renderTarget.getView();
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_testText);
	renderTarget.setView(oldView);
}

void MenuScreen::execOnEnter(const Screen *previousScreen)
{
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, 140.f));
	fireBasket2->setPosition(sf::Vector2f(998.f, 140.f));
	addObject(GameObjectType::_Undefined, fireBasket1);
	addObject(GameObjectType::_Undefined, fireBasket2);

	// add buttons
	m_newGameButton = new Button(sf::FloatRect(500, 250, 250, 90));
	m_newGameButton->setText(Texts::New_game);
	m_loadGameButton = new Button(sf::FloatRect(500, 350, 250, 90));
	m_loadGameButton->setText(Texts::Load_game);
	m_startGameButton = new Button(sf::FloatRect(500, 450, 250, 90));
	m_startGameButton->setText(Texts::Start_game);
	m_startGameButton->setEnabled(false);
	m_exitButton = new Button(sf::FloatRect(500, 550, 250, 90));
	m_exitButton->setText(Texts::Exit);
	addObject(GameObjectType::_Button, m_newGameButton);
	addObject(GameObjectType::_Button, m_loadGameButton);
	addObject(GameObjectType::_Button, m_exitButton);
	addObject(GameObjectType::_Button, m_startGameButton);

	m_testText = BitmapText(
		"TEST",
		(*g_resourceManager->getBitmapFont(ResourceID::BitmapFont_default)));

	m_testText.setColor(sf::Color::White);
	m_testText.setCharacterSize(30);
	m_testText.setPosition(sf::Vector2f(200, 300));
}

void MenuScreen::execOnExit(const Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_menu);
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash_fireanimation);
}