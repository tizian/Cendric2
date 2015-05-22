#include "Screens/MenuScreen.h"

using namespace std;

MenuScreen::MenuScreen(CharacterCore* core) : Screen(core)
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_menu)));
}

Screen* MenuScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || g_inputController->isMouseJustPressedLeft())
	{
		// we start a new game with an empty character core
		CharacterCore* core = (getCharacterCore() == nullptr) ? new CharacterCore() : m_characterCore;
		return new LoadingScreen(MapID::Testmap, core);
	}
	if (g_inputController->isMouseJustPressedRight() && m_characterCore == nullptr)
	{
		// TODO the .sav files should be loaded in another screen.
		char* saveFilename = "saves/testsave.sav";
		setTooltipText("Loading .sav file: " + string(saveFilename), sf::Vector2f(10.f, 10.f), sf::Color::Cyan);
		m_characterCore = new CharacterCore();
		if (!(m_characterCore->load(saveFilename)))
		{
			string errormsg = string(saveFilename) + ": save file corrupted!";
			g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		}
	}
	updateObjects(GameObjectType::_Undefined, frameTime);
	return this;
}

void MenuScreen::render(sf::RenderTarget &renderTarget) 
{
	renderTooltipText(renderTarget);
	setViewToStandardView(renderTarget);
	renderTarget.draw(m_screenSprite);
	renderObjects(GameObjectType::_Undefined, renderTarget);
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
}

void MenuScreen::execOnExit(const Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_menu);
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash_fireanimation);
}