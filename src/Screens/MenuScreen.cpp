#include "Screens\MenuScreen.h"

using namespace std;

MenuScreen::MenuScreen()
{
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_menu)));
}

Screen* MenuScreen::update(sf::Time frameTime)
{
	if (g_inputController->isKeyActive(Key::Escape) || g_inputController->isMouseJustPressedLeft())
	{
		return new LoadingScreen(ScreenID::Screen_map, ResourceID::Map_testmap);
	}
	updateObjects(GameObjectType::_Undefined, frameTime);
	return this;
}

void MenuScreen::render(sf::RenderTarget &renderTarget) 
{
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderObjects(GameObjectType::_Undefined, renderTarget);
}

void MenuScreen::execOnEnter(Screen *previousScreen)
{
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, 140.f));
	fireBasket2->setPosition(sf::Vector2f(998.f, 140.f));
	addObject(GameObjectType::_Undefined, fireBasket1);
	addObject(GameObjectType::_Undefined, fireBasket2);
}

void MenuScreen::execOnExit(Screen *nextScreen)
{
	g_resourceManager->deleteResource(ResourceID::Texture_screen_menu);
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash_fireanimation);
}