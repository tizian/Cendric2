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
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, static_cast<float>(WINDOW_HEIGHT) / (WINDOW_HEIGHT + BOTTOM_BORDER)));
	view.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	renderTarget.setView(view);
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